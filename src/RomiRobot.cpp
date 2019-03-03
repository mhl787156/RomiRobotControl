#import "RomiRobot.h"


void RomiInit(int debug) {

    // Setup Motor Controller
    mcMotorControllerInit(wheel_radius);
    mcSetPIDGains(0.5, 0, 0, 0.5, 0, 10);// In Air: 0.5, 0, 13, 0.5, 0, 14
    mcSetVelocityPIDGains(0.05, 0.01, 0.1, 0.05, 0.01, 0.1); //0.05, 0.01, 0.1
    mcSetDebug(debug == 2);


    // Set up buttons and lights
    pinMode(LED_Yellow_Pin, OUTPUT);
    pinMode(Button_A_Pin, INPUT);
    pinMode(Button_B_Pin, INPUT);
    pinMode(Button_C_Pin, INPUT);

    // Line Following PID:
    lsensor_heading_pid.setMax(lsensor_heading_delta_max);

    romi_debug = debug > 0;
    romi_initialised = true;
}

/*

Romi Motion and State Updates

*/
void RomiUpdateStateUsingICR(float left_motor_dist, float right_motor_dist) {
    float time_step = 1;
    float diff = right_motor_dist - left_motor_dist;
    float summ = right_motor_dist + left_motor_dist;
    float R = (base_width / 2) * (diff / summ);
    float wdt = diff * time_step / base_width;
    float ICCx = romi_loc_x - R * sin(romi_loc_theta);
    float ICCy = romi_loc_y - R * cos(romi_loc_theta);

    romi_loc_x = cos(wdt)*(romi_loc_x - ICCx) - sin(wdt)*(romi_loc_y - ICCy) + ICCx;
    romi_loc_y = sin(wdt)*(romi_loc_x - ICCx) + cos(wdt)*(romi_loc_y - ICCy) + ICCy;
    romi_loc_theta += wdt;

    if(romi_debug) {
        Serial.print("ICR -> ");
        RomiPrintState();
    }
}

void RomiMoveDistance(float millimeters) {

    // Move the motors
    mcMoveDistance(millimeters); 
    mcWaitDelayMoving(10);
    float left_motor_dist = mcGetDistanceLeft();
    float right_motor_dist = mcGetDistanceRight();
    float dist_travelled = (left_motor_dist + right_motor_dist)/2;

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


/* 

    Romi Motion Line Sensing

*/
bool RomiMoveForwardFindLine(float max_dist_millimeters) {
    bool foundLine = false;
    mcMoveDistance(max_dist_millimeters);
    while(mcIsMoving()) {

        if(lsensor.allOnLine()) {
            mcStopMotors();
            RomiBuzzBuzzer(0);
            foundLine = true;
        }
        delay(50);
    }
    float left_motor_dist = mcGetDistanceLeft();
    float right_motor_dist = mcGetDistanceRight();
    RomiUpdateStateUsingICR(left_motor_dist, right_motor_dist);
    if(!foundLine){RomiBuzzBuzzer(2);}
    return foundLine;
}

void RomiFollowLine() {
    float target_vel = PI * wheel_radius / 2;
    mcSetVelocity(target_vel); // Set Constant Velocity

    while (true) { // Update with end of line detection

        // Check if reached angular section and try to find line again
        if(!lsensor.anyOnLine()) {
            Serial.print("Reached CAP Searching...");
            RomiVelocityStop();
            bool foundLine = false;
            // Rotate the motors
            for (int i = 0; i < 4; i++) {
                delay(1000);
                mcRotateLeft(90);

                if (i == 1 || i == 2) { // rotated facing backwards dont do anything
                    mcWaitDelayMoving(10);
                } else {
                    while(mcIsMoving()){
                        if(lsensor.anyOnLine()) {
                            foundLine = true;
                            break;
                        }
                        delay(10);   
                    }
                }

                // Update State
                float left_motor_dist = mcGetDistanceLeft();
                float right_motor_dist = mcGetDistanceRight();
                float update = (right_motor_dist - left_motor_dist)/base_width;
                romi_loc_theta += update * 180 / PI;

                if (foundLine) {
                    Serial.print("Line refound");
                    break; // Continue following
                }
            }
            if(!foundLine) {
                Serial.print("End PATH");
                break; // End of Path
            }
        }

        mcVelocityControlLoop();

        float lsens_lr_error = lsensor.lineSensorError();
        float heading_ctrl = lsensor_heading_pid.update(lsens_lr_error);
        mcSetVelocityDelta(target_vel, heading_ctrl);
        
        Serial.print(lsens_lr_error);
        Serial.print(" ");
        Serial.print(heading_ctrl);
        Serial.println(" ");
        
        
        float left_motor_dist = mcGetDistanceLeftAndReset();
        float right_motor_dist = mcGetDistanceLeftAndReset();
        RomiUpdateStateUsingICR(left_motor_dist, right_motor_dist);

        delay(8);
    }

    RomiVelocityStop();

    RomiBuzzBuzzer(1);
}

void RomiVelocityStop() {
    mcSetVelocity(0);
    for (int i = 0; i < 100; i++) {
        mcVelocityControlLoop();
        delay(10);
    }
}

/* 

Debuggggg 

*/
void RomiPrintState() {
    Serial.print("x: ");
    Serial.print(romi_loc_x);
    Serial.print(" y: ");
    Serial.print(romi_loc_y);
    Serial.print(" t: ");
    Serial.println(romi_loc_theta);
}

void RomiPrintSensors() {
    lsensor.printSensors();
}

/* 

Buttons and Buzzers 

*/
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

void RomiBuzzBuzzer(int tune) {
    switch(tune) {
        case 0: buzzer.play("!L16 V8 cdefgab>cbagfedc"); break;
        case 1: buzzer.play("!T240 L8 agafaea dac+adaea fa<aa<bac#a dac#adaea f4"); break;
        case 2: buzzer.play("!V8 c<a-<f");break; //failure
        case 3: buzzer.play("!L4 V8 dc");
        default: buzzer.play("c");
    }
}


