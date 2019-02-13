#include <Arduino.h>
#line 1 "d:\\Documents\\coding_ground\\Arduino\\RomiRobotControl\\src\\RomiController.cpp"
#line 9 "d:\\Documents\\coding_ground\\Arduino\\RomiRobotControl\\src\\main.ino"
void setup();
#line 16 "d:\\Documents\\coding_ground\\Arduino\\RomiRobotControl\\src\\main.ino"
void loop();
#line 0 "d:\\Documents\\coding_ground\\Arduino\\RomiRobotControl\\src\\main.ino"
#line 1 "d:\\Documents\\coding_ground\\Arduino\\RomiRobotControl\\src\\RomiController.cpp"
#include "RomiController.h"


namespace Romi {

void init() {
    x = 0;
    y = 0;
    theta = 0;

    RomiEncoders::init();
    initialised = true;
}

void moveForward(float dist, float speed) {
    motors.moveForward(dist, speed);
    Serial.println("Moving Forward");
}

void rotateLeft(float degrees_angle) {
    motors.rotateLeft(degrees_angle);
    Serial.println("Rotating Left");
}

void readEncoders() {
    long cl = RomiEncoders::getLeftEncoderCount();
    long cr = RomiEncoders::getRightEncoderCount();
    Serial.print(cl);
    Serial.print(",");
    Serial.println(cr);
}




}
#line 1 "d:\\Documents\\coding_ground\\Arduino\\RomiRobotControl\\src\\main.ino"
// #include <Romi32U4.h>   // Romi
#include <LSM6.h>       // Romi IMU
#include <Wire.h>       // Romi IMU

#include "RomiController.h"

#define BAUD_RATE 9600

void setup() {
    Serial.begin(BAUD_RATE);
    Serial.println("***RESET***");
    delay(5000);
    Romi::init();
}

void loop() {
    // robot.readEncoders();
    // for(int i = 0; i < 10; i++){
        
        
    //     delay(10);
    // }
    Romi::moveForward(2.5, 50);
    Romi::readEncoders();
    delay(10);
    // Serial.println("bottom of loop");
    // while (true) {} // block 
}
