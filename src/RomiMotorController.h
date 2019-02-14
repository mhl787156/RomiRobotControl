#ifndef RomiMotorController_h
#define RomiMotorController_h

#include <arduino.h>
#include "RomiMotors.h"
#include "RomiEncoders.h"

class RomiMotorController {
    public:
        RomiMotorController(float wheel_radius);

        bool setMaxSpeed(float speed);
        bool moveDistance(float dist);
        bool rotateLeft(float angle);
        bool stopMotors();

        bool loopCheck();

    private:
        RomiMotors motors;
        float encoder_step;
        int one_wheel_revolution_count;
        int max_motor_speed = 50; // 0 to 255
        unsigned long mvmt_cut_off_time = 60 * 1000; // a minute

        unsigned long left_motor_mvmt_start_time;
        unsigned long right_motor_mvmt_start_time;
        long left_encoder_mvmt_count = 0;
        long right_encoder_mvmt_count = 0;

        bool moving = false;
        
        void resetCounters(long=0, long=0);
        bool moveCounts(long left, long right, bool action);

};




#endif // RomiMotorController_h