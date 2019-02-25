#ifndef RomiRobot_h
#define RomiRobot_h

#include <Arduino.h>
#include "MotorController.h"

static bool romi_initialised = false;
static bool romi_debug = false;


// Pose of Romi
static float romi_loc_x = 0;
static float romi_loc_y = 0;
static float romi_loc_theta = 0;

// Parameters of Romi
static float wheel_radius = 35;
static float base_width = 140;

// Miscellaneous
static const byte LED_Yellow_Pin = 13;
static const byte Button_A_Pin = 14;
static const byte Button_B_Pin = 30;
static const byte Button_C_Pin = 17;

void RomiInit(int debug);

// Functionality
void RomiMoveDistance(float millimeters);
void RomiRotateLeft(float deg);
bool RomiNavigateTo(float x, float y);
bool RomiGoHome(float x=0, float y=0);

// Helper Functions
void RomiPrintState();
bool RomiWaitUntilButtonAPressed();
byte RomiWhichButtonPressed();


#endif