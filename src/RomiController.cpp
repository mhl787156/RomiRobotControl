#include "RomiController.h"

Romi::Romi() {

}

void Romi::setSpeed(float new_speed) {
    motors.setSpeed(new_speed);
    Serial.println(motors.getSpeed());
}

void Romi::moveForward(float dist) {
    Serial.print("moving motors ");
    Serial.print(motors.getSpeed());
    Serial.print("\n");
    motors.moveForward(dist);
}