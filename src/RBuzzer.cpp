#import "RBuzzer.h"


void RBuzzer::playSequence(String s) {
    int len = s.length();
    for(int i = 0; i < len; i++) {
        playNote(s[i], duration);
    }
}


void RBuzzer::playNote(unsigned char note, unsigned int dur) {
    int freq = 0;
    switch(note){
        case 'a': freq = 220; break;
        case 'b': freq = 247; break;
        case 'c': freq = 262; break;
        case 'd': freq = 294; break;
        case 'e': freq = 330; break;
        case 'f': freq = 349; break;
        case 'g': freq = 392; break;
        case 'A': freq = 440; break;
        case 'B': freq = 494; break;
        case 'C': freq = 523; break;
        case 'D': freq = 587; break;
        case 'E': freq = 659; break;
        case 'F': freq = 698; break;
        case 'G': freq = 783; break;
    }
    playTone(freq, dur);  // set buzzer this freq/duration

}

void RBuzzer::playTone(unsigned int freq, unsigned int dur) {
    // tone(buzzerpin, freq, dur);
}

