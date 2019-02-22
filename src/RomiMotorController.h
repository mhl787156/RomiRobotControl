#ifndef RomiMotorController_h
#define RomiMotorController_h

#include <arduino.h>
#include "RomiMotors.h"
#include "RomiEncoders.h"

class RomiMotorController {
    public:
        RomiMotorController(float wheel_radius);
        
        void setPIDGains(double kp, double ki, double kd);
        bool setMaxSpeed(float speed);

        bool moveDistance(float dist);
        bool rotateLeft(float angle);
        bool stopMotors();

        bool loopCheck();

    private:
        RomiMotors motors;
        float encoder_step;
        int one_wheel_revolution_count;
        int max_motor_speed = 200; // mm/s
        int min_motor_speed = -max_motor_speed; // mm/s
        unsigned long mvmt_cut_off_time = 10 * 1000; // 20 seconds

        // PID Controller
        double kp,ki,kd = 0.1;
        long sum_of_left_errors = 0;
        long sum_of_right_errors = 0;
        long left_previous_error = 0;
        long right_previous_error = 0;
        unsigned long previous_pid_time = micros();

        // Stop checkers
        unsigned long left_motor_mvmt_start_time;
        unsigned long right_motor_mvmt_start_time;
        long left_encoder_mvmt_count = 0;
        long right_encoder_mvmt_count = 0;

        // Settling time check
        static const int num_loops_const_check = 5;
        int left_motor_error_history[num_loops_const_check] = {};
        int right_motor_error_history[num_loops_const_check] = {};
        int motor_history_error_idx = 0;

        bool moving = false;
        
        void resetCounters(long=0, long=0);
        bool moveCounts(long left, long right, bool action);

};




#endif // RomiMotorController_h