#include "RomiController.h"

Romi::Romi() {
    right_motor = new RomiMotor(LEFT_SPEED_PIN, LEFT_DIR_PIN);
    left_motor = new RomiMotor(RIGHT_SPEED_PIN, RIGHT_DIR_PIN);
}

void Romi::setSpeed(float new_speed) {
    float truncated_speed = constrain(new_speed, -max_speed, max_speed);
    speed = (int) (max_speed * truncated_speed / 100.0);
    Serial.println(speed);
}

void Romi::moveForward(float dist) {
    Serial.print("moving motors ");
    Serial.print(speed);
    Serial.print("\n");
    right_motor->setSpeed(speed);
    left_motor->setSpeed(speed);
    float k = dist/speed;
    Serial.println(k);
    delay(1000);
    right_motor->stop();
    left_motor->stop();
}