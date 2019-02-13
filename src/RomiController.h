#ifndef RomiController_h
#define RomiController_h

#include <Arduino.h>

#include "RomiMotors.h"
#include "RomiEncoders.h"

namespace Romi {

    bool initialised = false;

    // Location of Romi
    float x;
    float y;
    float theta;

    // Parameters of Romi
    float wheel_radius = 0.035; // m
    float base_width = 0.14; // m
    float encoder_step; // m, calculated in init()

    RomiMotors motors;


    // Functions of Romi
    void init();

    void moveForward(float dist, float speed);
    void rotateLeft(float degrees_angle);

    void readEncoders();
}


#endif // RomiController_h