#include <Arduino.h>
#include "RomiRobot.h"

#define BAUD_RATE 9600
#define DEBUG 1 // Off:0, Robot MEssages Only: 1 // Low Level PID Messages: 2


void setup_()
{
	Serial.begin(BAUD_RATE);
    delay(5000);

    RomiInit(DEBUG);

    Serial.println("Init Complete");
}

void testMovement1() {
    RomiMoveDistance(300);
    RomiRotateLeft(45);
    RomiMoveDistance(200);
    RomiRotateLeft(-60);
    RomiMoveDistance(200);
    // RomiNavigateTo(500, 500);
    RomiGoHome();
}

void testMovement2() {
    RomiMoveDistance(1500);
    RomiGoHome();
}

void loop_()
{   
    lsensor.printSensors();
    byte button = RomiWhichButtonPressed();
    if(button != 0){delay(1000);}
    if(button == 1) {
        RomiMoveForwardFindLine(1000);
    
    } else if(button == 2) {


    } else if (button == 3) {


    }

    if(button != 0 && DEBUG > 0) {
        Serial.print("Completed Trajectory ");
        Serial.println(button);
    }
    delay(100);
}
