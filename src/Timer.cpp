#include "Timer.h"

static int hztobitnumbers(int numhz) {
    bool cs32bit = numhz < 4;
    bool cs31bit = 244 > numhz && numhz >= 4;
    bool cs30bit = !( (244 > numhz && numhz > 31) ||
                    (4 > numhz && numhz > 0.95) );
    return (0 | (cs32bit << 2) | (cs31bit << 1) | cs30bit);
}

static float bitnumbertoprescale(int bitnumber) {
    int prescalepow = 2 * bitnumber;
    if (prescalepow <= 4) {prescalepow--;} // i.e. prescale of 8, 2^3 when bitvalue is 2*2
    if (prescalepow <= 2) {prescalepow--;} // i.e. prescale of 1, 2^0 when bitvalue is 1*2
    return pow(2, prescalepow);
}

void initTimer3(int numhz){
    cli(); //disble global interrupts

    //Reset timer3 to a blank condition
    // TCCR = Timer/Counter control Register
    TCCR3A = 0; // set enture TCCE3A register to 0
    TCCR3B = 0; // set entire TCCR3B register to 0

    // First, Turn on CTC mode. Timer 3 will count up and create an interrupt on a match to a value
    TCCR3B = TCCR3B | (1 << WGM32);

    // bool cs32bit = numhz < 4;
    // bool cs31bit = 244 > numhz && numhz >= 4;
    // bool cs30bit = !( (244 > numhz && numhz > 31) ||
    //                 (4 > numhz && numhz > 0.95) );

    int bitnumber = hztobitnumbers(numhz);

    // int prescalepow = 2*(0 | (cs32bit << 2) | (cs31bit << 1) | cs30bit);
    // if (prescalepow <= 4) {prescalepow--;} // i.e. prescale of 8, 2^3 when bitvalue is 2*2
    // if (prescalepow <= 2) {prescalepow--;} // i.e. prescale of 1, 2^0 when bitvalue is 1*2
    // float prescale = pow(2, prescalepow);

    float prescale = bitnumbertoprescale(bitnumber);

    // For a cpu clock prescaler of 256
    // Left shift a 1 up to bit CS32 (clock select, timer 3, bit 2)
    // TCCR3B = TCCR3B | ( cs32bit << CS32) | (cs31bit << CS31) | (cs30bit << CS30);
    TCCR3B = TCCR3B | bitnumber; // all clock select bits on the end of TCCR3B

    // set compare match register to desired timer count.
    OCR3A = (unsigned int) ((16e6/prescale)/numhz);

    // enable timer compare interrupt:
    TIMSK3 = TIMSK3 | (1 << OCIE3A);

    // enable global interrupts:
    sei();
}



