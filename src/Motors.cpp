#include "Motors.h"

void mMotorsInit() {
    pinMode(LEFT_DIR_PIN, OUTPUT);
    pinMode(LEFT_SPEED_PIN, OUTPUT);
    pinMode(RIGHT_DIR_PIN, OUTPUT);
    pinMode(RIGHT_SPEED_PIN, OUTPUT);
}


void _mMotorSet(int motor_value, const byte dir_pin, const byte speed_pin){
    motor_value = constrain(motor_value, -m_max_motor_value, m_max_motor_value);
    if (motor_value >= 0) {
        digitalWrite(dir_pin, LOW);
    } else {
        digitalWrite(dir_pin, HIGH);
    }
    analogWrite(speed_pin, abs(motor_value));
}

// Left Motor
void mLeftMotorSet(int motor_value) { // -255 to 255
    _mMotorSet(motor_value, LEFT_DIR_PIN, LEFT_SPEED_PIN);
}

// Right Motor
void mRightMotorSet(int motor_value) { // -255 to 255
    _mMotorSet(motor_value, RIGHT_DIR_PIN, RIGHT_SPEED_PIN);
}

//Both Motors
void mSetMotors(int left, int right) {
    mLeftMotorSet(left);
    mRightMotorSet(right);
}

void mStopMotors() {
    mSetMotors(0, 0);
}
