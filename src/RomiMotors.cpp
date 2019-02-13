#include "RomiMotors.h"


/*
BOTH ROMI MOTORS
*/
RomiMotors::RomiMotors() {
    right_motor = new RomiMotor(LEFT_SPEED_PIN, LEFT_DIR_PIN);
    left_motor = new RomiMotor(RIGHT_SPEED_PIN, RIGHT_DIR_PIN);
}

void RomiMotors::setSpeed(float new_speed) {
    float truncated_speed = constrain(new_speed, -max_speed, max_speed);
    speed = (int) (max_speed * truncated_speed / 100.0);
}

void RomiMotors::moveForward(float dist) {
    right_motor->setSpeed(speed);
    left_motor->setSpeed(speed);
    float k = dist/speed;
    delay(dist * 100);
    right_motor->stop();
    left_motor->stop();
}

void RomiMotors::moveForward(float dist, float speed) {
    setSpeed(speed);
    moveForward(dist);
}


/*
 SINGLE ROMI MOTOR
*/
RomiMotor::RomiMotor(const byte speedPin, const byte dirPin) {
    speed_pin = speedPin;
    dir_pin = dirPin;
    pinMode(speed_pin, OUTPUT);
    pinMode(dir_pin, OUTPUT);
}

void RomiMotor::setSpeed(int speed) {
    if (speed >= 0) {
        digitalWrite(dir_pin, LOW);    
    } else {
        digitalWrite(dir_pin, HIGH);
    }

    analogWrite(speed_pin, speed);
}

void RomiMotor::stop() {
    analogWrite(speed_pin, 0);
}




