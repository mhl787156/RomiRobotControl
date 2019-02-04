#include "RomiController.h"


void Romi::setSpeed(float speed) {
    float truncated_percentage = constrain(speed, -300, 300);
    speed = (int16_t) (max_speed * truncated_percentage / 100.0);
    Serial.println(speed);
}

void Romi::moveForward(float dist) {
    motors.setLeftSpeed(speed);
    motors.setRightSpeed(speed);
    delay(1000);
    motors.setLeftSpeed(0);
    motors.setRightSpeed(0);
    delay(1000);
}