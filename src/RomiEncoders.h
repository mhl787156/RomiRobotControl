#ifndef RomiEconder_h
#define RomiEconder_h

// #include <FastGPIO.h>
#include <Arduino.h>


static volatile bool old_left_A ;
static volatile bool old_left_B ;
static volatile bool old_right_A;
static volatile bool old_right_B;

static volatile long count_left;
static volatile long count_right;

const int left_A_xor_pin = 26;//8;
const int left_B_pin = PINE & (1 << PINE2); // IO_E2;
const int right_A_xor_pin = 7;
const int right_B_pin = 23;

class RomiEncoders {
    public:
        RomiEncoders();

        void leftEncoderInterruptSetup();
        void rightEncoderInterruptSetup();

        void checkLeftEncoder();
        void checkRightEncoder();
        float getLeftEncoderCount(){
                cli();
                long c = count_left;
                sei();
                return c;
            }
        float getRightEncoderCount(){
                cli();
                long c = count_right;
                sei();
                return c;
            }
        float resetEncoderCount() { 
                cli();
                count_left = 0;
                count_right = 0;
                sei();
            }

};

#endif