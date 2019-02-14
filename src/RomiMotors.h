#ifndef RomiMotors_h
#define RomiMotors_h

#include <Arduino.h>


class RomiMotor {
    public:
        RomiMotor(const byte speed_pin, const byte dir_pin);
        void setSpeed(int speed);
        void stop();
    private:
        byte speed_pin;
        byte dir_pin;
};

class RomiMotors {
    public:
        const int gear_ratio = 120;

        RomiMotors();

        // set speed as percentage
        void setSpeed(float speed); 
        int getSpeed() {return speed;}

        void moveForward(float dist, float speed);
        void moveForward(float dist);
        void rotateLeft(float degAngle);

        void moveLeft(float speed);
        void moveRight(float speed);
        void move(float speed);

        void stopLeft();
        void stopRight();
        void stop();

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

#endif