#ifndef RomiController_h
#define RomiController_h

#include <Romi32U4.h>  
#include "Arduino.h"

class Romi {

    public:

        // Romi();

        // set speed as percentage
        void setSpeed(float speed); 

        void moveForward(float dist);
        void rotateLeft(float degAngle);

    private:
        const int16_t max_speed = 300;
        int16_t speed; //value between -300 and 300

        Romi32U4Motors motors;

};

#endif // RomiController_h