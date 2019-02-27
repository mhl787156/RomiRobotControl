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

static float mc_max_motor_speed = 80;
static int mc_motor_dead_zone = 5;
static float lki, lkp, lkd = 0.0;
static float rki, rkp, rkd = 0.0;

// Interrupt used variables
static const int mc_mvmt_cut_off_time = 10 * 1000; // 10 seconds
static volatile bool mc_robot_moving = false;
static unsigned long mc_left_mvmt_start_time;
static unsigned long mc_right_mvmt_start_time;

// PID Variables
static volatile long mc_left_encoder_target_count = 0;
static volatile long mc_right_encoder_target_count = 0;
static volatile PID left_encoder_pid(0,0,0);
static volatile PID right_encoder_pid(0,0,0);
static volatile int mc_left_error = 0;
static volatile int mc_right_error = 0;

// Initialisation
void mcMotorControllerInit(float wheel_radius);
void mcSetPIDGains(float _lkp, float _lki, float _lkd, float _rkp, float _rki, float _rkd);
void mcSetDebug(bool _debug);

// functions 
bool mcMoveDistance(float dist); // Millimeters using position pid of both left and right
bool mcRotateLeft(float deg); // Degrees using position pid of both left and right
bool mcStopMotors(); 
bool mcIsMoving();
void mcWaitDelayMoving(int dly);
float mcGetDistanceLeft();
float mcGetDistanceRight();

bool _mcMoveCounts(long left, long right);
void _mcResetCounters(long left, long right);

// interrupt function
bool mcMotorControlLoop();




#endif