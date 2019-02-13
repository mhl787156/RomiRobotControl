#ifndef RomiEconder_h
#define RomiEconder_h

#include <Arduino.h>

namespace RomiEncoders {

static volatile bool old_left_A ;
static volatile bool old_left_B ;
static volatile bool old_right_A;
static volatile bool old_right_B;

static volatile long count_left;
static volatile long count_right;

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
}

#endif //RomiEconder_h