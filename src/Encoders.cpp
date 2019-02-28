#include "Encoders.h"

// Interrupt Functions
void eCheckLeftEncoder() {
    bool new_B = PINE & ( 1 << PINE2 ); //reading left B pin
    bool new_A = digitalRead(LEFT_A_XOR_PIN) ^ new_B;

    // Check Direction, Increment Count
    byte state = 0 | (new_A << 3) | (new_B << 2) | 
                     (e_old_left_A << 1) | e_old_left_B;
    if (state == 1 || state == 7 || state == 8 || state == 14) {
        e_count_left--; // Moving Backwards
        e_left_tick_counter--; // Counter for count velocity
    } else if (state == 2 || state == 4 || state == 11 || state == 13) {
        e_count_left++; // Moving Forwards
        e_left_tick_counter++; // Counter for count velocity
    }
    
    e_old_left_B = new_B;
}

void eCheckRightEncoder() {
    bool new_B = digitalRead(RIGHT_B_PIN);
    bool new_A = digitalRead(RIGHT_A_XOR_PIN) ^ new_B;

    // Check Direction, Increment Count
    byte state = 0 | (new_A << 3) | (new_B << 2) | 
                     (e_old_right_A << 1) | e_old_right_B;
    if (state == 1 || state == 7 || state == 8 || state == 14) {
        e_count_right--; // Moving Backwards
        e_right_tick_counter--; // Counter for count velocity
    } else if (state == 2 || state == 4 || state == 11 || state == 13) {
        e_count_right++; // Moving Forwards
        e_right_tick_counter++; // Counter for count velocity
    }

    e_old_right_A = new_A;
    e_old_right_B = new_B;
}

// left encoder interrupt routine
ISR(PCINT0_vect) {
    eCheckLeftEncoder();
}

// right encoder intertupt routine
ISR(INT6_vect) {
    eCheckRightEncoder();
}


// Initialisation
void eEncoderInit() {
    eLeftEncoderInterruptSetup();
    eRightEncoderInterruptSetup();
}

void eLeftEncoderInterruptSetup() {
    // Set Direct Data Register to use pin B
    DDRE = DDRE & ~(1<<DDE6);
    // Setup pin B
    PORTE |= (1 << PORTE2);

    pinMode(LEFT_A_XOR_PIN, INPUT);
    digitalWrite(LEFT_A_XOR_PIN, HIGH);

    PCICR = PCICR & ~(1 << PCIE0);
    PCMSK0 |= (1 << PCINT4);
    PCIFR |= (1 << PCIF0);
    PCICR |= (1 <<PCIE0);
}

void eRightEncoderInterruptSetup() {
    pinMode(RIGHT_A_XOR_PIN, INPUT);
    pinMode(RIGHT_B_PIN, INPUT);

    // External Interrupt Mask Register EIMSK
    EIMSK = EIMSK & ~( 1 << INT6 );

    //External Interrupt Control Register B EICRB
    EICRB |= ( 1 << ISC60 );

    //External Interrupt Flag Register EIFR
    EIFR |= (1 << INTF6);

    //External Interrupt Mask Register EIMSK
    EIMSK |= (1 << INT6);
}

// Volatile Getters and Setters
long eGetLeftEncoderCount() {
    cli();
    long c = e_count_left;
    sei();
    return c;
}

long eGetLeftEncoderCountAndReset() {
    cli();
    long c = e_count_left;
    e_count_left = 0;
    sei();
    return c;
}

long eGetRightEncoderCount() {
    cli();
    long c = e_count_right;
    sei();
    return c;
}

long eGetRightEncoderCountAndReset() {
    cli();
    long c = e_count_right;
    e_count_left = 0;
    sei();
    return c;
}

long eResetEncoderCount() {
    cli();
    e_count_left = 0;
    e_count_right = 0;
    sei();
}

float eGetLeftTickSpeed() {
    long curr_micros = micros();
    long time_past = curr_micros - e_left_prev_micros;
    cli();
    int s = e_left_tick_counter;
    e_left_tick_counter = 0;
    sei();
    e_left_prev_micros = curr_micros;
    return (s * 1e6) / time_past;
}

float eGetRightTickSpeed() {
    long curr_micros = micros();
    long time_past = curr_micros - e_right_prev_micros;
    cli();
    int s = e_right_tick_counter;
    e_right_tick_counter = 0;
    sei();
    e_right_prev_micros = curr_micros;
    return (s * 1e6)/time_past;
}