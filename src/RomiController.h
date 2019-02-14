#ifndef RomiController_h
#define RomiController_h

#include <Arduino.h>

#include "RomiMotorController.h"

namespace Romi {

    bool initialised = false;

    // Location of Romi
    float x;
    float y;
    float theta;

    // Parameters of Romi
    float wheel_radius = 0.035; // m
    float base_width = 0.14; // m
    float speed = 50;

    // Movement
    RomiMotorController romiMotorControls(wheel_radius);

    bool loopCheck();
}


#endif // RomiController_h