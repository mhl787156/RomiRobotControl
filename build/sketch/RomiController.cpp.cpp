#include <Arduino.h>
#line 1 "d:\\Documents\\coding_ground\\Arduino\\RomiRobotControl\\src\\RomiController.cpp"
#line 11 "d:\\Documents\\coding_ground\\Arduino\\RomiRobotControl\\src\\main.ino"
void setup();
#line 19 "d:\\Documents\\coding_ground\\Arduino\\RomiRobotControl\\src\\main.ino"
void loop();
#line 0 "d:\\Documents\\coding_ground\\Arduino\\RomiRobotControl\\src\\main.ino"
#line 1 "d:\\Documents\\coding_ground\\Arduino\\RomiRobotControl\\src\\RomiController.cpp"
#include "RomiController.h"

Romi::Romi() {
    right_motor = new RomiMotor(LEFT_SPEED_PIN, LEFT_DIR_PIN);
    left_motor = new RomiMotor(RIGHT_SPEED_PIN, RIGHT_DIR_PIN);
}

void Romi::setSpeed(float new_speed) {
    float truncated_speed = constrain(new_speed, -max_speed, max_speed);
    speed = (int) (max_speed * truncated_speed / 100.0);
    Serial.println(speed);
}

void Romi::moveForward(float dist) {
    Serial.print("moving motors ");
    Serial.print(speed);
    Serial.print("\n");
    right_motor->setSpeed(speed);
    left_motor->setSpeed(speed);
    delay((int) (dist/speed));
    right_motor->stop();
    left_motor->stop();
}
#line 1 "d:\\Documents\\coding_ground\\Arduino\\RomiRobotControl\\src\\main.ino"
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
