#include "RomiEncoders.h"

namespace RomiEncoders {

void checkLeftEncoder() {
    bool new_B = PINE & ( 1 << PINE2 ); //reading left B pin
    bool new_A = digitalRead(left_A_xor_pin) ^ new_B;
    byte state = 0 | (new_A << 3) | (new_B << 2) | 
                     (old_left_A << 1) | old_left_B;
    if (state == 1 || state == 7 || state == 8 || state == 14) {
        count_left--; // Moving Backwards
    } else if (state == 2 || state == 4 || state == 11 || state == 13) {
        count_left++; // Moving Forwards
    }
    old_left_A = new_A;
    old_left_B = new_B;
}

void checkRightEncoder() {
    bool new_B = digitalRead(right_B_pin);
    bool new_A = digitalRead(right_A_xor_pin) ^ new_B;
    byte state = 0 | (new_A << 3) | (new_B << 2) | 
                     (old_right_A << 1) | old_right_B;
    if (state == 1 || state == 7 || state == 8 || state == 14) {
        count_right--; // Moving Backwards
    } else if (state == 2 || state == 4 || state == 11 || state == 13) {
        count_right++; // Moving Forwards
    }
    old_right_A = new_A;
    old_right_B = new_B;
}

// left encoder interrupt routine
ISR(PCINT0_vect) {
    checkLeftEncoder();
}

// right encoder interrupt routine
ISR(INT6_vect) {
    checkRightEncoder();
}

void leftEncoderInterruptSetup() {
    count_left = 0;
    old_left_A = false;
    old_left_B = false;

    // Set Direct Data Register to use pin B
    DDRE = DDRE & ~(1<<DDE6);
    // Setup pin B
    PORTE |= (1 << PORTE2);

    pinMode(left_A_xor_pin, INPUT);
    digitalWrite(left_A_xor_pin, HIGH);

    PCICR = PCICR & ~(1 << PCIE0);
    PCMSK0 |= (1 << PCINT4);
    PCIFR |= (1 << PCIF0);
    PCICR |= (1 <<PCIE0);
}

void rightEncoderInterruptSetup() {
    pinMode(right_A_xor_pin, INPUT);
    pinMode(right_B_pin, INPUT);

    count_right = 0;
    old_right_A = false;
    old_right_B = false;

    // External Interrupt Mask Register EIMSK
    EIMSK = EIMSK & ~( 1 << INT6 );

    //External Interrupt Control Register B EICRB
    EICRB |= ( 1 << ISC60 );

    //External Interrupt Flag Register EIFR
    EIFR |= (1 << INTF6);

    //External Interrupt Mask Register EIMSK
    EIMSK |= (1 << INT6);
}

void init() {
    leftEncoderInterruptSetup();
    rightEncoderInterruptSetup();
}

long getLeftEncoderCount(){
        cli();
        long c = count_left;
        sei();
        return c;
}

long getRightEncoderCount(){
        cli();
        long c = count_right;
        sei();
        return c;
}

long getAndResetRightEncoderCount() { 
        cli();
        long c = count_right;
        count_right = 0;
        sei();
        return c;
}

long getAndResetLeftEncoderCount() { 
        cli();
        long c = count_left;
        count_left = 0;
        sei();
        return c;
}

void resetEncoderCount() { 
        cli();
        count_left = 0;
        count_right = 0;
        sei();
}

}