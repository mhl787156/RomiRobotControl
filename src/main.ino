// #include <Romi32U4.h>   // Romi
#include <LSM6.h>       // Romi IMU
#include <Wire.h>       // Romi IMU

#include "RomiController.h"

#define BAUD_RATE 9600

int task = 0;
const int num_tasks = 1;
bool (*tasks[num_tasks]) () {
    // [](){return Romi::romiMotorControls.rotateLeft(90);},
    [](){return Romi::romiMotorControls.moveDistance(1000);}
};


void terminate() {
    while(true){};
}

void setup() {
    Serial.begin(BAUD_RATE);
    Serial.println("***RESET***");
    delay(5000);
    Romi::init();
    Romi::romiMotorControls.setPIDGains(0.6, 0.6, 0);
}

void loop() {

    // Check if all tasks run
    if( task >= num_tasks ) {
        // Serial.println("Tasks Finished");
        // terminate(); // Could also repeat by setting task = 0
        task = 0;
    }

    // Run Current Movement Task
    // if( !(*tasks[task])() ) {
        // Serial.println("Function returned false");
    // }
    Romi::romiMotorControls.moveDistance(1000);

    Romi::loopCheck();
    // Serial.print(task);
    // Serial.println(" completed");

    delay(5000);
    task++;

}