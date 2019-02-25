#ifndef MotorController_h
#define MotorController_h


#include "Motors.h"
#include "Encoders.h"
#include "Timer.h"
#include "PIDController.h"

static bool debug = false;

static float mc_wheel_radius = 0;
static int mc_counts_per_wheel_revolution = 0;
static float mc_encoder_step = 0;

static float mc_max_motor_speed = 200;
static float lki, lkp, lkd = 0.0;
static float rki, rkp, rkd = 0.0;

// Interrupt used variables
static const int mc_mvmt_cut_off_time = 10 * 1000; // 10 seconds
static volatile bool mc_robot_moving = false;
static unsigned long mc_left_mvmt_start_time;
static unsigned long mc_right_mvmt_start_time;

// PID Variables
static volatile PID left_encoder_pid(0,0,0);
static volatile PID right_encoder_pid(0,0,0);
static volatile long mc_left_encoder_target_count;
static volatile long mc_right_encoder_target_count;
static volatile long mc_sum_of_left_errors = 0;
static volatile long mc_sum_of_right_errors = 0;
static volatile long mc_left_previous_error = 0;
static volatile long mc_right_previous_error = 0;
static volatile unsigned long mc_previous_pid_time = micros();

// Settling time check
static const int mc_num_loops_const_check = 3;
static volatile int mc_left_motor_error_history[mc_num_loops_const_check] = {};
static volatile int mc_right_motor_error_history[mc_num_loops_const_check] = {};
static volatile int mc_motor_history_error_idx = 0;

// Initialisation
void mcMotorControllerInit(float wheel_radius);
void mcSetPIDGains(float _lkp, float _lki, float _lkd, float _rkp, float _rki, float _rkd);
void mcSetDebug(bool _debug);

// functions 
bool mcMoveDistance(float dist); // Millimeters
bool mcRotateLeft(float deg); // Degrees
bool mcStopMotors();
bool mcIsMoving();
void mcWaitDelayMoving(int dly);

bool _mcMoveCounts(long left, long right);
void _mcResetCounters(long left, long right);

// interrupt function
bool mcMotorControlLoop();




#endif