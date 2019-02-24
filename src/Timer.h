#ifndef TIMER_H
#define TIMER_H

#include "Arduino.h"

static int hztobitnumbers(int hz);
static float bitnumbertoprescale(int bitnumber); 

void initTimer3(int);
#endif
