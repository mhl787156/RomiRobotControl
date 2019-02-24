#include "RomiMotorController.h"

RomiMotorController::RomiMotorController(float wheel_radius) {
    RomiEncoders::init();
    one_wheel_revolution_count = RomiEncoders::counts_per_revolution * motors.gear_ratio;
    encoder_step = (2 * PI * wheel_radius) / one_wheel_revolution_count;
}

bool RomiMotorController::moveDistance(float dist) {
    long dist_in_counts = (long) (dist/encoder_step);
    return moveCounts(dist_in_counts, dist_in_counts, true);
}

bool RomiMotorController::rotateLeft(float angle) {
    float err = 0;
    float dist = 2*one_wheel_revolution_count * ((angle+err) / (360));
    long dist_in_counts = (long) dist;
    return moveCounts(dist_in_counts, dist_in_counts, false);
}

bool RomiMotorController::moveCounts(long left, long right, bool action) {
    //action true->forward, false->rotate
    resetCounters(left, right);

    RomiEncoders::resetEncoderCount();

    int left_motor_speed = (action?1:-1) * max_motor_speed;
    int right_motor_speed = max_motor_speed;
    // Serial.println(left_motor_speed);
    // Serial.println(right_motor_speed);
    // motors.moveLeft(left_motor_speed);
    // motors.moveRight(max_motor_speed);
    moving = true;
    return true;
}

void RomiMotorController::resetCounters(long left = 0, long right = 0) {
    left_encoder_mvmt_count = left;
    right_encoder_mvmt_count = right;
    unsigned long curr_time = millis();
    left_motor_mvmt_start_time = curr_time;
    right_motor_mvmt_start_time = curr_time;
}

bool RomiMotorController::stopMotors() {
    motors.stop();
    resetCounters();
    moving = false;
}

void RomiMotorController::setPIDGains(double _kp, double _ki, double _kd) {
    kp = _kp; ki = _ki; kd = _kd;
}

bool RomiMotorController::loopCheck() {
    long left_count = abs(RomiEncoders::getLeftEncoderCount());
    long right_count = abs(RomiEncoders::getRightEncoderCount());

    // Find encoder count errors
    long left_distance_error = left_encoder_mvmt_count - left_count;
    long right_distance_error = right_encoder_mvmt_count - right_count;

    // Check if reached required count value
    bool mvmt_count_reached = left_distance_error <= 0 
                                && right_distance_error <= 0
                                && left_encoder_mvmt_count > 0
                                && right_encoder_mvmt_count > 0;

    // Check if settled
    left_motor_error_history[motor_history_error_idx] = left_distance_error;
    right_motor_error_history[motor_history_error_idx] = right_distance_error;
    motor_history_error_idx = (motor_history_error_idx + 1) % num_loops_const_check;

    int left_history_errors = 0;
    int right_history_errors = 0;
    for(int i = 0; i < num_loops_const_check; i++) {
        left_history_errors += abs(left_motor_error_history[i]);
        right_history_errors += abs(right_motor_error_history[i]);
    }
    bool contoller_settled = left_history_errors < 1e-3 
                                && right_history_errors < 1e-3;

    // Check Reached maximum time limit for movement
    unsigned long curr_time = millis();
    bool time_lim_reached = curr_time - left_motor_mvmt_start_time > mvmt_cut_off_time
                        ||  curr_time - right_motor_mvmt_start_time > mvmt_cut_off_time;
    

    // Implement PID in here on count differences
    // if (moving) {
        // Proportional Controller
        double left_velocity = kp * left_distance_error;
        double right_velocity = kp * right_distance_error;

        // Integral Controller
        sum_of_left_errors += left_distance_error/100;
        left_velocity += ki * sum_of_left_errors;

        sum_of_right_errors += right_distance_error/100;
        right_velocity += ki * sum_of_right_errors;

        // Differential Controller
        unsigned long time_since_last = micros() - previous_pid_time;
        if(left_previous_error != 0) {
            left_velocity += kd * (left_distance_error - left_previous_error)/time_since_last;
            left_previous_error = left_distance_error;
        }
        if(left_previous_error != 0) {
            right_velocity += kd * (right_distance_error - right_previous_error)/time_since_last;
            right_previous_error = right_distance_error;
        }

        left_velocity = constrain(left_velocity, min_motor_speed, max_motor_speed);
        right_velocity = constrain(right_velocity, min_motor_speed, max_motor_speed);

        // Serial.print(RomiEncoders::getLeftTickSpeed());
        // Serial.print(" ");
        // Serial.print(RomiEncoders::getRightTickSpeed());
        // Serial.print(" ");
        Serial.print(sum_of_left_errors);
        Serial.print(" ");
        Serial.print(sum_of_right_errors);
        Serial.print(" ");
        Serial.print(left_velocity);
        Serial.print(" ");
        Serial.print(right_velocity);
        Serial.print(" ");
        Serial.print(left_encoder_mvmt_count);
        Serial.print(" ");
        Serial.print(right_encoder_mvmt_count);
        Serial.print(" ");
        Serial.print(left_count);
        Serial.print(" ");
        Serial.println(right_count);

        motors.moveLeft(motors.leftDirection() * left_velocity);
        motors.moveRight(motors.rightDirection() * right_velocity);
    // }

    if (time_lim_reached) {
        stopMotors(); return true;
    }

    // If settled
    if (contoller_settled) {
    // if (contoller_settled && moving) {
        // stopMotors(); No need to stop motors
        // moving = false;
        return true;
    }
    return false;
}
