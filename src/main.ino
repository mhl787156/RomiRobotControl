// #include <Romi32U4.h>   // Romi
#include <LSM6.h>       // Romi IMU
#include <Wire.h>       // Romi IMU

#include "RomiController.h"

#define BAUD_RATE 9600

Romi robot;

void setup() {
    Serial.begin(BAUD_RATE);
    Serial.println("***RESET***");
    delay(5000);

    robot.setSpeed(50.0);
}

void loop() {
    delay(1000);
    robot.moveForward(10.0);
    Serial.println("moving");
    while (true) {} // block 
}