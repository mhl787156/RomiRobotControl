#import "LineSensor.h"


// Roll from white to black, we record min and max values
bool LineSensor::calibrate() {
    Triplet<float> pins = readSensors();
    float min_l, min_c, min_r = 100000;
    float max_l, max_c, max_r = 0;
    int diff = 500;
    int num_loops = 1000;

    for(int i = 0; i < num_loops; i++) {
        pins = readSensors();

        min_l = min(min_l, pins.x);
        min_c = min(min_c, pins.y);
        min_r = min(min_r, pins.z);
    
        max_l = max(max_l, pins.x);
        max_c = max(max_c, pins.y);
        max_r = max(max_r, pins.z);

        // If central pin difference is large, and pin low again
        if ((max_c - min_c) > diff && pins.y < (min_c + diff) ){
            break;
        }

        delay(10);
    } 

    if((max_c - min_c) < diff) {
        return false;
    }

    cut_off_l = 2/3 * (max_l - min_l);
    cut_off_c = 2/3 * (max_c - min_c);
    cut_off_r = 2/3 * (max_r - min_r);
    return true;
}

// Reads the sensors
Triplet<float> LineSensor::readSensors() {
    float lpin = analogRead(right_pin); // right
    float cpin = analogRead(centre_pin); //centre
    float rpin = analogRead(left_pin); //left
    return Triplet<float>(lpin, cpin, rpin);
}

// Reads the sensors and converts to digital via calibrated cut off
Triplet<bool> LineSensor::readDigitalSensor() {
    Triplet<float> pin = readSensors();
    return Triplet<bool>(pin.x > cut_off_l, pin.y>cut_off_c, pin.z>cut_off_r);
}

// Returns true if any sensor reads a line
bool LineSensor::anyOnLine() {
    Triplet<bool> pins = readDigitalSensor();
    return pins.x || pins.y || pins.z;
}
    
// Returns true if all sensors read a line
bool LineSensor::allOnLine() {
    Triplet<bool> pins = readDigitalSensor();
    return pins.x && pins.y && pins.z;
}

// Returns difference between outer pins, assuming on line.
// Used for rotation pid control
float LineSensor::lineSensorError() {
    Triplet<float> pins = readSensors();
    return pins.z - pins.x;
}