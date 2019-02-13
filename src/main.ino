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