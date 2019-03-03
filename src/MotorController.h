#ifndef MotorController_h
#define MotorController_h


#include "Motors.h"
#include "Encoders.h"
#include "Timer.h"
#include "PIDController.h"

static bool mc_debug = false;

static float mc_wheel_radius = 0;
static int mc_counts_per_wheel_revolution = 0;
static float mc_encoder_step = 0;

static float mc_max_motor_speed = 100;
static int mc_motor_dead_zone = 5;
static float lki, lkp, lkd = 0.0;
static float rki, rkp, rkd = 0.0;

// Interrupt used variables
static const int mc_mvmt_cut_off_time = 10 * 1000; // 10 seconds
static volatile bool mc_robot_moving = false;
static unsigned long mc_left_mvmt_start_time;
static unsigned long mc_right_mvmt_start_time;

// Position PID Variables
static volatile long mc_left_encoder_target_count = 0;
static volatile long mc_right_encoder_target_count = 0;
static volatile PID left_encoder_pid(0,0,0);
static volatile PID right_encoder_pid(0,0,0);

// Velocity PID Variables
static volatile float mc_left_velocity_target = 0;
static volatile float mc_right_velocity_target = 0;
static volatile PID left_velocity_pid(0,0,0);
static volatile PID right_velocity_pid(0,0,0);

// For debug
static volatile float mc_control_left = 0;
static volatile float mc_control_right = 0;
float mcGetControlRight();



// Initialisation
void mcMotorControllerInit(float wheel_radius);
void mcSetPIDGains(float _lkp, float _lki, float _lkd, float _rkp, float _rki, float _rkd);
void mcSetVelocityPIDGains(float _lkp, float _lki, float _lkd, float _rkp, float _rki, float _rkd);
void mcSetDebug(bool _debug);

// Distance Since Last Getters
float mcGetDistanceLeft();
float mcGetDistanceRight();
float mcGetDistanceLeftAndReset();
float mcGetDistanceRightAndReset();

// Generic functions
bool mcStopMotors(); 
bool mcIsMoving();
void mcWaitDelayMoving(int dly);

// position functions 
bool mcMoveDistance(float dist); // Millimeters using position pid of both left and right
bool mcRotateLeft(float deg); // Degrees using position pid of both left and right
bool _mcMoveCounts(long left, long right);
void _mcResetCounters(long left, long right);

// velocity functions
void mcSetVelocity(float velocity); // in mm/s
void mcSetVelocityCounts(float counts_per_sec); // in counts/s
void mcSetVelocityDelta(float velocity, float velocityDelta);


// interrupt function
bool mcMotorControlLoop();
void mcVelocityControlLoop();



#endif