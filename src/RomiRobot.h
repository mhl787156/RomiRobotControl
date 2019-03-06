#ifndef RomiRobot_h
#define RomiRobot_h

#include <Arduino.h>
#include "MotorController.h"
#include "LineSensor.h"
#include "PIDController.h"
#include "RBuzzer.h"

static bool romi_initialised = false;
static bool romi_debug = false;

// Pose of Romi
static float romi_loc_x = 0;
static float romi_loc_y = 0;
static float romi_loc_theta = 0;

// Parameters of Romi
static float wheel_radius = 35;
static float base_width = 140;

// Sensors
static LineSensor lsensor;
static const byte lsensor_heading_delta_max = 150;
static PID lsensor_heading_pid(0.5,0,0);

// Miscellaneous
// static RBuzzer rbuzzer;
// static PololuBuzzer buzzer;
static const byte Buzzer_Pin = 6;
static const byte LED_Yellow_Pin = 13;
static const byte Button_A_Pin = 14;
static const byte Button_B_Pin = 30;
static const byte Button_C_Pin = 17;

void RomiInit(int debug);
void RomiResetState();

// Specific Task Functionality
bool RomiMoveForwardFindLine(float max_dist_millimeters); //Todo
void RomiFollowLine(bool endfind=true); //Todo
void RomiICRDebug();

// Functionality
void RomiUpdateStateUsingICR(float left_motor_dist, float right_motor_dist);
void RomiMoveDistance(float millimeters);
void RomiRotateLeft(float deg);
bool RomiNavigateTo(float x, float y);
bool RomiGoHome(float x=0, float y=0);
void RomiVelocityGo();
void RomiVelocityStop();

// Helper Functions
void RomiPrintState();
void RomiPrintSensors();
bool RomiWaitUntilButtonAPressed();
byte RomiWhichButtonPressed();
void RomiBuzzBuzzer(int tune); 


#endif