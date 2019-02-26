#import "RomiRobot.h"


void RomiInit(int debug) {

    // Setup Motor Controller
    mcMotorControllerInit(wheel_radius);
    mcSetPIDGains(0.5, 0, 14, 0.5, 0, 15);// In Air: 0.5, 0, 13, 0.5, 0, 14
    mcSetDebug(debug == 2);


    // Set up buttons and lights
    pinMode(LED_Yellow_Pin, OUTPUT);
    pinMode(Button_A_Pin, INPUT);
    pinMode(Button_B_Pin, INPUT);
    pinMode(Button_C_Pin, INPUT);

    romi_debug = debug > 0;
    romi_initialised = true;
}

void RomiMoveDistance(float millimeters) {

    // Move the motors
    mcMoveDistance(millimeters); 
    mcWaitDelayMoving(10);
    float left_motor_dist = mcGetDistanceLeft();
    float right_motor_dist = mcGetDistanceRight();
    float dist_travelled = (left_motor_dist + right_motor_dist)/2;
    Serial.print(dist_travelled);

    // Update State
    romi_loc_x += dist_travelled * cos(romi_loc_theta);
    romi_loc_y += dist_travelled * sin(romi_loc_theta);
    
    if(romi_debug) {
        Serial.print("F -> ");
        RomiPrintState();
    }
}

void RomiRotateLeft(float deg) {

    // Move the motors
    mcRotateLeft(deg);
    mcWaitDelayMoving(10);
    float left_motor_dist = mcGetDistanceLeft();
    float right_motor_dist = mcGetDistanceRight();

    // Update State
    float update = (right_motor_dist - left_motor_dist)/base_width;
    romi_loc_theta += update * 180 / PI;
    
    if(romi_debug) {
        Serial.print("R -> ");
        RomiPrintState();
    }
}

bool RomiNavigateTo(float x, float y){
    // Work out distance from origin
    float dx = x - romi_loc_x; // targetx - x
    float dy = y - romi_loc_y; // targety - y

    Serial.print(dy);
    Serial.print(" ");
    Serial.print(dx);
    Serial.print(" ");

    // Calculate trajectory parameters
    float target_dist = sqrt( (dx*dx) + (dy*dy) );
    float target_angle = atan2(dy, dx) * 360 / PI;
    
    Serial.print(target_dist);
    Serial.print(" ");
    Serial.println(target_angle);

    // Move Parameters
    RomiRotateLeft(target_angle);
    RomiMoveDistance(target_dist);
}

bool RomiGoHome(float x=0, float y=0) {
    // Go to specified home position
    RomiNavigateTo(x, y);

    // Straighten up
    RomiRotateLeft(-romi_loc_theta);

    Serial.println("HOME");
}



void RomiPrintState() {
    Serial.print("x: ");
    Serial.print(romi_loc_x);
    Serial.print(" y: ");
    Serial.print(romi_loc_y);
    Serial.print(" t: ");
    Serial.println(romi_loc_theta);
}

bool RomiWaitUntilButtonAPressed() {
    if(romi_debug){Serial.println("Waiting for Button A");}
    while(digitalRead(Button_A_Pin) == HIGH) {
        delay(10);
    }
    digitalWrite(Button_A_Pin, HIGH);
    if(romi_debug){Serial.println("Button A Pressed");}
    delay(1000);
}

byte RomiWhichButtonPressed() {
    byte output = 0;
    if (digitalRead(Button_B_Pin) == LOW) {
        if(romi_debug){Serial.println("Button B Pressed");}
        output = 2; digitalWrite(Button_B_Pin, HIGH);
    } else if (digitalRead(Button_C_Pin) == LOW) {
        if(romi_debug){Serial.println("Button C Pressed");}
        output = 3; digitalWrite(Button_C_Pin, HIGH);
    } else if (digitalRead(Button_A_Pin) == LOW) {
        if(romi_debug){Serial.println("Button A Pressed");}
        output = 1; digitalWrite(Button_A_Pin, HIGH);
    } 
    return output;
}
