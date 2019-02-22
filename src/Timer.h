#ifndef TIMER_H
#define TIMER_H

#include "Arduino.h"

class Timers {
    public:
        Timers();
        //Timer 1
        void runTimer1func();

        //Timer 2
        void runTimer2func();

        // Timer 3
        void addInterruptTimer3(void (*) (void), int hz);
        void setTimer3func(void (*) (void));
        void initTimer3(int);
        void runTimer3func();
    private:
        void (*timer3func) (void); // function pointer for timer3

        int hztobitnumbers(int hz);
        float bitnumbertoprescale(int bitnumber); 
};

#endif
