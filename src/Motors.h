#ifndef Motors_H
#define Motors_H

#include <Arduino.h>

const byte LEFT_SPEED_PIN = 10;
const byte LEFT_DIR_PIN = 16;
const byte RIGHT_SPEED_PIN = 9;
const byte RIGHT_DIR_PIN = 15;


// Motors
const byte m_max_motor_value = 255;
const byte m_min_motor_value = 0;
const int m_gear_ratio = 120;

void mMotorsInit();

void _mMotorSet(int motor_value, const byte dir_pin, const byte speed_pin);

// Left Motor
void mLeftMotorSet(int motor_value); // -255 to 255

// Right Motor
void mRightMotorSet(int motor_value); // -255 to 255

// Both Motors
void mSetMotors(int left, int right);
void mStopMotors();

#endif