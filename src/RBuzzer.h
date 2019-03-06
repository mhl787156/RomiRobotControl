#ifndef Buzzer_h
#define Buzzer_h

#import <arduino.h>

class RBuzzer {
    public:
        void playSequence(String s);
        void playNote(unsigned char note, unsigned int dur);
        
    private:
        const byte buzzerpin = 6;
        unsigned int duration = 500; // ms
        
        void playTone(unsigned int freq, unsigned int dur);


};

#endif