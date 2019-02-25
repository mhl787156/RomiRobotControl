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

    // Set PIDs
    mcSetPIDGains(0.5, 0, 13, 0.5, 0, 14); // Pre found values
    left_encoder_pid.setMax(mc_max_motor_speed);
    right_encoder_pid.setMax(mc_max_motor_speed);

    // Initialise Motor Control Loop timer at 20 Hz
    initTimer3(20); 
}

void mcSetPIDGains(float _lkp, float _lki, float _lkd, float _rkp, float _rki, float _rkd) {
    lkp = _lkp; lki = _lki; lkd = _lkd;
    rkp = _rkp; rki = _rki; rkd = _rkd;
    left_encoder_pid.setGains(_lkp, _lkd, _lki);
    right_encoder_pid.setGains(_rkp, _rkd, _rki);
}

void mcSetDebug(bool _debug) {
    mc_debug = _debug;
    left_encoder_pid.setDebug(_debug);
    right_encoder_pid.setDebug(_debug);
}


// Move Distance 
bool mcMoveDistance(float dist){
    long dist_in_counts = (long) (dist/mc_encoder_step);
    return _mcMoveCounts(dist_in_counts, dist_in_counts);
}

// Rotate Left
bool mcRotateLeft(float deg){
    float err = 0;
    float dist_in_counts = (long) (mc_counts_per_wheel_revolution * ((deg+err)/ 180));
    return _mcMoveCounts(-dist_in_counts, dist_in_counts);
}

// Emergency Motor Stop
bool mcStopMotors(){
    mStopMotors();
    mc_robot_moving = false;
    return true;
}

// Get Final Counts and translate into distance moved
float mcGetDistanceLeft() {
    cli();
    float v = (mc_left_encoder_target_count - mc_left_error) * mc_encoder_step;
    sei();
    return v;
}

float mcGetDistanceRight(){
    cli();
    float v = (mc_right_encoder_target_count - mc_right_error) * mc_encoder_step;
    sei();
    return v;
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
    unsigned long curr_time = millis();
    cli();
    mc_left_encoder_target_count = left;
    mc_right_encoder_target_count = right;
    left_encoder_pid.reset();
    right_encoder_pid.reset();
    mc_left_error = 0;
    mc_right_error = 0;
    mc_left_mvmt_start_time = curr_time;
    mc_right_mvmt_start_time = curr_time;
    sei();
}

// Move a specified number of counts
bool _mcMoveCounts(long left, long right) {
    _mcResetCounters(left, right);
    eResetEncoderCount();
    mc_robot_moving = true;
}

// PID Loop
bool mcMotorControlLoop() {
    if(mc_robot_moving){
        // Check Time Limit Reached
        unsigned long curr_time = millis();
        bool time_lim_reached =  curr_time - mc_left_mvmt_start_time > mc_mvmt_cut_off_time
                            ||  curr_time - mc_right_mvmt_start_time > mc_mvmt_cut_off_time;

        if(time_lim_reached) {
            if(mc_debug) {Serial.println("Time lim reached");}
            mcStopMotors();
            return true; // continue on loop
        }

        // Read Encoder Counts
        long left_count = eGetLeftEncoderCount();
        long right_count = eGetRightEncoderCount();
        mc_right_error = mc_right_encoder_target_count - right_count;
        mc_left_error = mc_left_encoder_target_count - left_count;

        // PID update
        float pid_class_left_vel = left_encoder_pid.update(mc_left_error);
        float pid_class_right_vel = right_encoder_pid.update(mc_right_error);

        // PID within dead zone
        if(left_encoder_pid.settled(mc_left_error) && right_encoder_pid.settled(mc_right_error)) {
            if(mc_debug) {Serial.println("Settled");}
            mcStopMotors();
            return true;
        }

        // Set motors PID control velocity
        mSetMotors(pid_class_left_vel, pid_class_right_vel);
    }
    return false;
}


// ISR 
ISR( TIMER3_COMPA_vect ) {
    if (mcMotorControlLoop()) {
        // Run Function which runs next function....
    }
}
