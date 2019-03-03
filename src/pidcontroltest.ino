#include <Arduino.h>
#include "MotorController.h"

#define BAUD_RATE 9600
#define DEBUG 1 // Off:0, Robot MEssages Only: 1 // Low Level PID Messages: 2


void setup_()
{
	Serial.begin(BAUD_RATE);
    delay(3000);
    mcMotorControllerInit(35);
    mcSetVelocityPIDGains(0.05, 0.01, 0.1, 0.05, 0.01, 0.1); //0.05, 0.01, 0.1
    mcSetDebug(true);
    Serial.println("Init Complete");
}


void loop_()
{   
    mcSetVelocity(PI * 35);
    
    for(int i = 0; i < 200; i++) {
        mcVelocityControlLoop();
        delay(10);
    }

    // for(int j = 0; j < 5; j++) {
    //     mcSetVelocityDelta(j*5);
    //     for(int i = 0; i < 50; i++) {
         
    //         mcVelocityControlLoop();
    //         delay(10);
    //     }
    // }
    
    // for(int j = 0; j < 5; j++) {
    //     mcSetVelocityDelta(-j*5);
    //     for(int i = 0; i < 50; i++) {
         
    //         mcVelocityControlLoop();
    //         delay(10);
    //     }
    // }
    
    // for(int j = 0; j < 5; j++) {
    //     mcSetVelocityDelta(-j*5);
    //     for(int i = 0; i < 50; i++) {
         
    //         mcVelocityControlLoop();
    //         delay(10);
    //     }
    // }

    // for(int j = 0; j < 5; j++) {
    //     mcSetVelocityDelta(j*5);
    //     for(int i = 0; i < 50; i++) {
         
    //         mcVelocityControlLoop();
    //         delay(10);
    //     }
    // }
}
