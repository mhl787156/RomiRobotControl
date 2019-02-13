#include "RomiController.h"


namespace Romi {

void init() {
    x = 0;
    y = 0;
    theta = 0;

    RomiEncoders::init();
    initialised = true;
}

void moveForward(float dist, float speed) {
    motors.moveForward(dist, speed);
    Serial.println("Moving Forward");
}

void rotateLeft(float degrees_angle) {
    motors.rotateLeft(degrees_angle);
    Serial.println("Rotating Left");
}

void readEncoders() {
    long cl = RomiEncoders::getLeftEncoderCount();
    long cr = RomiEncoders::getRightEncoderCount();
    Serial.print(cl);
    Serial.print(",");
    Serial.println(cr);
}




}