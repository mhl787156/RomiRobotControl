#include <Arduino.h>
#include "MotorController.h"

#define BAUD_RATE 9600
#define DEBUG false

void setup()
{
	Serial.begin(BAUD_RATE);
    delay(5000);
    mcMotorControllerInit(35);
    mcSetPIDGains(0.5, 0, 14, 0.5, 0, 15);// 0.5, 0, 13, 0.5, 0, 14
    mcSetDebug(true);
    Serial.println("Init Complete");
}

void loop()
{
    Serial.println("Moving Forward");
    mcMoveDistance(300); 
    mcWaitDelayMoving(10);

    mcRotateLeft(180);
    mcWaitDelayMoving(10);

    Serial.println("Finished Moving");
    delay(3000);

    // mcRotateLeft(90);
    // mcWaitDelayMoving(10);

    // delay(1000);
    // while(true){}
}
