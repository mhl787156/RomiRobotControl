#ifndef RomiController_h
#define RomiController_h

#include <Arduino.h>
// #include <Romi32U4.h>  

#include "RomiMotors.h"
#include "RomiEncoders.h"

class Romi {

    public:

        Romi();
        
        // Movement
        void setSpeed(float speed); 
        void moveForward(float dist);
        void rotateLeft(float degAngle);

        // Encoders
        void readEncoders() {
            Serial.println("Encoder Readings");
            Serial.println(encoders.getLeftEncoderCount());
            Serial.println(encoders.getRightEncoderCount());
        }

    private:
        RomiMotors motors;
        RomiEncoders encoders;

};




#endif // RomiController_h