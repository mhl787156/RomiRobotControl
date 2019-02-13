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