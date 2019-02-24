#ifndef RomiController_h
#define RomiController_h

#include <Arduino.h>

#include "RomiMotorController.h"
#include "Timer.h"


namespace Romi {

    bool initialised = false;

    // Location of Romi
    float x;
    float y;
    float theta;

    // Parameters of Romi
    float wheel_radius = 35; // mm
    float base_width = 140; // mm
    float speed = 50;

    // Movement
    RomiMotorController romiMotorControls(wheel_radius);

    // Timers
    // Timers interruptTimer();

    bool loopCheck();
}


#endif // RomiController_h