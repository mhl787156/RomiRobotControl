#include "RomiController.h"

RomiMotor::RomiMotor(const byte speedPin, const byte dirPin) {
    speed_pin = speedPin;
    dir_pin = dirPin;
    pinMode(speed_pin, OUTPUT);
    pinMode(dir_pin, OUTPUT);
}

void RomiMotor::setSpeed(int speed) {
    if (speed >= 0) {
        digitalWrite(dir_pin, HIGH);    
    } else {
        digitalWrite(dir_pin, LOW);
    }

    analogWrite(speed_pin, speed);
}

void RomiMotor::stop() {
    analogWrite(speed_pin, 0);
}
