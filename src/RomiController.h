#ifndef RomiController_h
#define RomiController_h

#include <Arduino.h>
#include <Romi32U4.h>  


class RomiMotor {
    public:
        RomiMotor(const byte speed_pin, const byte dir_pin);
        void setSpeed(int speed);
        void stop();
    public:
        byte speed_pin;
        byte dir_pin;
};


class Romi {

    public:

        Romi();

        // set speed as percentage
        void setSpeed(float speed); 

        void moveForward(float dist);
        void rotateLeft(float degAngle);

    private:
        const int max_speed = 100;
        int speed; //value between -100 and 100

        const byte LEFT_SPEED_PIN = 10;
        const byte LEFT_DIR_PIN = 16;
        const byte RIGHT_SPEED_PIN = 9;
        const byte RIGHT_DIR_PIN = 15;

        RomiMotor* right_motor;
        RomiMotor* left_motor;
};




#endif // RomiController_h