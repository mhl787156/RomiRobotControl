#ifndef LineSensor_H
#define LineSensor_H

#include <Arduino.h>
#include "utils.h"

class LineSensor {

    public:
        
        bool calibrate(); // Calibrate line level

        bool anyOnLine(); // Returns true if any sensor reads a line
        bool allOnLine(); // Returns true if all sensors read a line

        float lineSensorError(); // Returns Ir - Il as error signal;


    private:
        const byte left_pin = A4;
        const byte centre_pin = A3;
        const byte right_pin = A2;

        float cut_off_l = 600.0;
        float cut_off_c = 600.0;
        float cut_off_r = 600.0;

        Triplet<float> readSensors();
        Triplet<bool> readDigitalSensor();
        
};


#endif