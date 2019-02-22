#include "RomiController.h"


namespace Romi {

void init() {
    x = 0;
    y = 0;
    theta = 0;

    // romiMotorControls = RomiMotorController(wheel_radius);
    // timer.setTimer3func(&loopCheck)
    // timer.initTimer3(100) // Run LoopCheck at 100 times

    initialised = true;
}

// Runs a delay loop until conditions are true (busy wait)
bool loopCheck() {
    while (true) {
        bool conditions = romiMotorControls.loopCheck();

        if(conditions) {
            Serial.println("Finished");
            break;
        }
        delay(10);
    }
}


}