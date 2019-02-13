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

}

void Romi::setSpeed(float new_speed) {
    motors.setSpeed(new_speed);
    Serial.println(motors.getSpeed());
}

void Romi::moveForward(float dist) {
    Serial.print("moving motors ");
    Serial.print(motors.getSpeed());
    Serial.print("\n");
    motors.moveForward(dist);
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
    robot.readEncoders();
    // for(int i = 0; i < 10; i++){
        
    //     robot.moveForward(2.5);
    delay(10);
    // }
        
    // Serial.println("bottom of loop");
    // while (true) {} // block 
}
