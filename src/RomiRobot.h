#ifndef RomiRobot_h
#define RomiRobot_h

// #include <Arduino.h>
#include "MotorController.h"

bool romi_initialised = false;

// Pose of Romi
float romi_loc_x = 0;
float romi_loc_y = 0;
float romi_loc_theta = 0;

// Parameters of Romi
float wheel_radius = 35;
float base_width = 140;




#endif