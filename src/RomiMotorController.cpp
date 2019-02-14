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
    motors.moveLeft(left_motor_speed);
    motors.moveRight(max_motor_speed);
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

bool RomiMotorController::loopCheck() {
    long left_count = abs(RomiEncoders::getLeftEncoderCount());
    long right_count = abs(RomiEncoders::getRightEncoderCount());

    bool mvmt_count_reached = left_count >= left_encoder_mvmt_count
                                && right_count >= right_encoder_mvmt_count
                                && left_encoder_mvmt_count > 0
                                && right_encoder_mvmt_count > 0;
    unsigned long curr_time = millis();
    bool time_lim_reached = curr_time - left_motor_mvmt_start_time > mvmt_cut_off_time
                        ||  curr_time - right_motor_mvmt_start_time > mvmt_cut_off_time;
    
    // Serial.print(left_encoder_mvmt_count);
    // Serial.print(", ");
    // Serial.print(right_encoder_mvmt_count);
    // Serial.print(", ");
    // Serial.print(left_count);
    // Serial.print(", ");
    // Serial.println(right_count);

    // Implement PID in here on count differences

    // If reached count, or time_lim_reached or moving interrupted, stop
    if (mvmt_count_reached || time_lim_reached || !moving) {
        stopMotors();
        return true;
    }
    return false;
}
