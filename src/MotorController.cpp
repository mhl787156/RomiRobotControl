#include "MotorController.h"
#include "utils.h"

void mcMotorControllerInit(float wheel_radius){

    // Init Motors and Encoders
    mMotorsInit();
    eEncoderInit();

    // Calculate useful parameters
    mc_wheel_radius = wheel_radius;
    mc_counts_per_wheel_revolution = e_counts_per_revolution * m_gear_ratio;
    mc_encoder_step = (2 * PI * mc_wheel_radius) / mc_counts_per_wheel_revolution;

    // Initialise timer at 10 Hz
    initTimer3(10); 
}

void mcSetPIDGains(float _lkp, float _lki, float _lkd, float _rkp, float _rki, float _rkd) {
    lkp = _lkp; lki = _lki; lkd = _lkd;
    rkp = _rkp; rki = _rki; rkd = _rkd;
    left_encoder_pid.setGains(_lkp, _lki, _lkd);
    right_encoder_pid.setGains(_rkp, _rki, _rkd);
}

void mcSetDebug(bool _debug) {
    debug = _debug;
}


// Move Distance 
bool mcMoveDistance(float dist){
    long dist_in_counts = (long) (dist/mc_encoder_step);
    return _mcMoveCounts(dist_in_counts, dist_in_counts);
}

// Rotate Left
bool mcRotateLeft(float deg){
    float err = 0;
    float dist_in_counts = (long) (2 * mc_counts_per_wheel_revolution * ((deg+err)/ 360));
    return _mcMoveCounts(-dist_in_counts, dist_in_counts);
}

// Emergency Motor Stop
bool mcStopMotors(){
    mStopMotors();
    mc_robot_moving = false;
    return true;
}

// Is Robot Moving
bool mcIsMoving() {
    return mc_robot_moving;
}

// Busy Wait For Moving
void mcWaitDelayMoving(int dly) {
    while(mcIsMoving()) {delay(dly);}
}

// Reset Target Counters and timers
void _mcResetCounters(long left = 0, long right = 0) {
    cli();
    mc_left_encoder_target_count = left;
    mc_right_encoder_target_count = right;
    sei();
    unsigned long curr_time = millis();
    mc_left_mvmt_start_time = curr_time;
    mc_right_mvmt_start_time = curr_time;
}

// Move a specified number of counts
bool _mcMoveCounts(long left, long right) {
    _mcResetCounters(left, right);
    eResetEncoderCount();
    mc_robot_moving = true;
}

// Timer Interrupt based PID Control Loop;
bool mcMotorControlLoop() {
    
    // Only do Control if the robot is moving
    if (mc_robot_moving) {
        // Check Time Lim Reached
        unsigned long curr_time = millis();
        bool time_lim_reached =  curr_time - mc_left_mvmt_start_time > mc_mvmt_cut_off_time
                            ||  curr_time - mc_right_mvmt_start_time > mc_mvmt_cut_off_time;

        if(time_lim_reached) {
            Serial.println("Time lim reached");
            mcStopMotors();
            mc_robot_moving = false;
            return true; // continue on loop
        }

        // Read Encoder Counts
        long left_count = eGetLeftEncoderCount();
        long right_count = eGetRightEncoderCount();

        // Get Encoder Count Errors
        int left_distance_error = mc_left_encoder_target_count - left_count;
        int right_distance_error = mc_right_encoder_target_count - right_count;

        // Settling Time Check
        mc_left_motor_error_history[mc_motor_history_error_idx] = left_distance_error;
        mc_right_motor_error_history[mc_motor_history_error_idx] = right_distance_error;
        mc_motor_history_error_idx = (mc_motor_history_error_idx + 1) % mc_num_loops_const_check;

        int left_history_errors = 0;
        int right_history_errors = 0;
        for(int i = 0; i < mc_num_loops_const_check; i++) {
            left_history_errors += abs(mc_left_motor_error_history[i]);
            right_history_errors += abs(mc_right_motor_error_history[i]);
        } // If Sum of previous errors is small
        bool controller_settled =  left_history_errors < 1 
                                && right_history_errors < 1;
        if(controller_settled) {
            Serial.println("Settled");
            mc_robot_moving = false;
            return true;
        }

        float pid_class_left_vel = left_encoder_pid.update(mc_left_encoder_target_count, left_count);
        float pid_class_right_vel = right_encoder_pid.update(mc_right_encoder_target_count, right_count);

        // Calculate PID Values
        // Proportional Controller
        float left_velocity = lkp * left_distance_error;
        float right_velocity = rkp * right_distance_error;
        
        // Integral Controller
        mc_sum_of_left_errors += left_distance_error;
        left_velocity += lki * mc_sum_of_left_errors;

        mc_sum_of_right_errors += right_distance_error;
        right_velocity += rki * mc_sum_of_right_errors;

        // Differential Controller
        unsigned long time_since_last = micros() - mc_previous_pid_time;
        if(mc_left_previous_error != 0) {
            left_velocity += lkd * (left_distance_error - mc_left_previous_error)/time_since_last;
            mc_left_previous_error = left_distance_error;
        }
        if(mc_right_previous_error != 0) {
            right_velocity += rkd * (right_distance_error - mc_right_previous_error)/time_since_last;
            mc_right_previous_error = right_distance_error;
        }

        // Constrain Velocities
        left_velocity = constrain(left_velocity, -mc_max_motor_speed, mc_max_motor_speed);
        right_velocity = constrain(right_velocity, -mc_max_motor_speed, mc_max_motor_speed);

        // Set the motors
        mSetMotors(left_velocity, right_velocity);

        // Debugging
        if (debug) {
            Serial.print(mc_left_encoder_target_count);
            Serial.print(" ");
            Serial.print(mc_right_encoder_target_count);
            Serial.print(" ");
            Serial.print(left_count);
            Serial.print(" ");
            Serial.println(right_count);
        }
    }
    return false;
}

// ISR 
ISR( TIMER3_COMPA_vect ) {
    if (mcMotorControlLoop()) {
        // Run Function which runs next function....
    }
}
