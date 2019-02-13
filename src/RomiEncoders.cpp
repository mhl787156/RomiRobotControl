#include "RomiEncoders.h"


void checkEncoder(int pin_A, int pin_B, volatile bool& old_A, volatile bool& old_B, volatile long* count) {
    bool new_B = digitalRead(pin_B);
    bool new_A = digitalRead(pin_A) ^ new_B;
    byte state = 0 | (new_A << 3) | (new_B << 2) | 
                     (old_A << 1) | old_B;
    if (state == 1 || state == 7 || state == 8 || state == 14) {
        (*count)++;
    } else if (state == 2 || state == 4 || state == 11 || state == 13) {
        (*count)--;
    }
    Serial.print(new_A);
    Serial.print(" ");
    Serial.print(new_B);
    Serial.print(old_A);
    Serial.print(" ");
    Serial.print(old_B);
    Serial.print("\n");
    old_A = new_A;
    old_B = new_B;
}

// left encoder interrupt routine
ISR(PCINT0_vect) {
    checkEncoder(left_A_xor_pin, left_B_pin, old_left_A, old_left_B, &count_left);
}

// right encoder interrupt routine
ISR(INT6_vect) {
    checkEncoder(right_A_xor_pin, right_B_pin, old_right_A, old_right_B, &count_right);
}

RomiEncoders::RomiEncoders() {
    leftEncoderInterruptSetup();
    rightEncoderInterruptSetup();
}

void RomiEncoders::leftEncoderInterruptSetup() {
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

void RomiEncoders::rightEncoderInterruptSetup() {
    pinMode(right_A_xor_pin, INPUT);
    pinMode(right_B_pin, INPUT);

    count_right = 0;
    old_right_A = false;
    old_right_B = false;

    // External Interrupt Mask Register EIMSK
    EIMSK = EIMSK & ~ ( 1 << INT6 );

    //External Interrupt Control Register B EICRB
    EICRB |= (1 << INT6);

    //External Interrupt Flag Register EIFR
    EIFR |= (1 << INTF6);

    //External Interrupt Mask Register EIMSK
    EIMSK |= (1 << INT6);
}


void RomiEncoders::checkLeftEncoder() {
    checkEncoder(left_A_xor_pin, left_B_pin, old_left_A, old_left_B, &count_left);

}

void RomiEncoders::checkRightEncoder() {
    checkEncoder(right_A_xor_pin, right_B_pin, old_right_A, old_right_B, &count_right);
}

