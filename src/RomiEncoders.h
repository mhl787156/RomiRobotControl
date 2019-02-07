#ifndef RomiEconder_h
#define RomiEconder_h


#include <FastGPIO.h>
#include <Arduino.h>


class RomiEncoders {
    public:

    private:
        const int left_xor_pin = 8;
        const int left_B = IO_E2;
        const int right_xor_pin = 7;
        const int right_B = 23;
};


#endif