#include <Arduino.h>
#line 1 "g:\\OnedriveBusiness\\OneDrive - University of Bristol\\Documents\\Arduino\\roboticSystems\\RobotControl\\RomiController.cpp"
#line 1 "g:\\OnedriveBusiness\\OneDrive - University of Bristol\\Documents\\Arduino\\roboticSystems\\RobotControl\\RomiController.cpp"
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
#line 1 "g:\\OnedriveBusiness\\OneDrive - University of Bristol\\Documents\\Arduino\\roboticSystems\\RobotControl\\main.ino"
// #include <Romi32U4.h>   // Romi
#include <LSM6.h>       // Romi IMU
#include <Wire.h>       // Romi IMU

#include "RomiController.h"

#define BAUD_RATE 9600

Romi robot;

void setup() {
    Serial.begin(BAUD_RATE);
    Serial.println("***RESET***");

    delay(500);
    // robot = new Romi();
    robot.setSpeed(20.0);
}

void loop() {
    delay(1000);
    robot.moveForward(10.0);
}
