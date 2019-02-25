#include <Arduino.h>
#include "MotorController.h"

#define BAUD_RATE 9600

void setup()
{
	Serial.begin(BAUD_RATE);
    delay(5000);
    mcMotorControllerInit(35);
    mcSetPIDGains(0.3, 0.00, 0.04, 0.3, 0, 0.04);
    mcSetDebug(debug);
    Serial.println("Init Complete");
}

void loop()
{
    Serial.println("Moving Forward");
    mcMoveDistance(500); 
    mcWaitDelayMoving(10);

    mcRotateLeft(90);
    mcWaitDelayMoving(10);

    Serial.println("Finished Moving");
    delay(3000);

    // mcRotateLeft(90);
    // mcWaitDelayMoving(10);

    // delay(1000);
    // while(true){}
}
