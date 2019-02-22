#ifndef RomiEconder_h
#define RomiEconder_h

#include <Arduino.h>

namespace RomiEncoders {

const int counts_per_revolution = 12;

static volatile bool old_left_A  = false;
static volatile bool old_left_B  = false;
static volatile bool old_right_A = false;
static volatile bool old_right_B = false;

static volatile long count_left  = 0;
static volatile long count_right = 0;

static volatile unsigned long left_prev_micros = micros();
static volatile unsigned long right_prev_micros = micros();
static const int num_speed_history = 5;
static volatile float left_speeds[num_speed_history] = {} ; // ticks p sec
static volatile float right_speeds[num_speed_history] = {};
static volatile int left_speed_idx = 0;
static volatile int right_speed_idx = 0;


const int left_A_xor_pin = 26;//8;
const int right_A_xor_pin = 7;
const int right_B_pin = 23;


void checkLeftEncoder();
void checkRightEncoder();

void init();
void leftEncoderInterruptSetup();
void rightEncoderInterruptSetup();

long getLeftEncoderCount();
long getRightEncoderCount();
long getAndResetRightEncoderCount();
long getAndResetLeftEncoderCount() ;
void resetEncoderCount();

float getLeftTickSpeed(); //instantaneous velocity
float getRightTickSpeed();
}

#endif //RomiEconder_h