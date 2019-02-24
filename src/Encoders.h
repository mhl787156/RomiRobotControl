#ifndef Encoders_H
#define Encoders_H

#include <Arduino.h>

static const int e_counts_per_revolution = 12;

// Encoder Pins
const byte LEFT_A_XOR_PIN = 26;
const byte RIGHT_A_XOR_PIN = 7;
const byte RIGHT_B_PIN = 23;

// Encoder Counting
static volatile bool e_old_left_A;
static volatile bool e_old_left_B;
static volatile bool e_old_right_A;
static volatile bool e_old_right_B;

static volatile long e_count_left;
static volatile long e_count_right;

// Encoder Speed
static volatile unsigned long e_left_prev_micros = micros();
static volatile unsigned long e_right_prev_micros = micros();
static const int e_num_speed_history = 5;
static volatile float e_left_speeds[e_num_speed_history] = {} ; // ticks p sec
static volatile float e_right_speeds[e_num_speed_history] = {};
static volatile int e_left_speed_idx = 0;
static volatile int e_right_speed_idx = 0;

// Interrupt Functions
void eCheckLeftEncoder();
void eCheckRightEncoder();

// Initialisation
void eEncoderInit();
void eLeftEncoderInterruptSetup();
void eRightEncoderInterruptSetup();

// Volatile Getters and Setters
long eGetLeftEncoderCount();
long eGetRightEncoderCount();
long eResetEncoderCount();
float eGetLeftTickSpeed(); //instantaneous velocity
float eGetRightTickSpeed();

#endif