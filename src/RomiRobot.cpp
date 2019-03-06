#import "RomiRobot.h"
#import "utils.h"
#define PRINT(x) Serial.print(x, 4); Serial.print(" ");

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

void RomiResetState() {
    romi_loc_x = 0;
    romi_loc_y = 0;
    romi_loc_theta = 0;
}
/*

Romi Motion and State Updates

*/
void RomiICRDebug() {
    while(true) {
        float left_motor_dist = mcGetDistanceLeftAndReset();
        float right_motor_dist = mcGetDistanceRightAndReset();
        // Update time step TODO
        RomiUpdateStateUsingICR(left_motor_dist, right_motor_dist);
        delay(100);
    }
}

void RomiUpdateStateUsingICR(float left_motor_dist, float right_motor_dist) {
    float diff = right_motor_dist - left_motor_dist;
    float summ = right_motor_dist + left_motor_dist;

    if(abs(diff) < 1e-6) { // Forward Motion
        float dist_travelled = (left_motor_dist + right_motor_dist)/2;
        romi_loc_x += dist_travelled * cos(romi_loc_theta);
        romi_loc_y += dist_travelled * sin(romi_loc_theta);
    } else if (abs(summ) < 1e-6) { // On spot
        float update = (right_motor_dist - left_motor_dist)/base_width;
        romi_loc_theta += update;
    } else { // Use ICC

        float R = (base_width / 2) * (summ / diff);
        float wdt = diff / base_width; // time_step from ms to s
        float ICCx = romi_loc_x - R * sin(romi_loc_theta);
        float ICCy = romi_loc_y + R * cos(romi_loc_theta);

        romi_loc_x = cos(wdt)*(romi_loc_x - ICCx) - sin(wdt)*(romi_loc_y - ICCy) + ICCx;
        romi_loc_y = sin(wdt)*(romi_loc_x - ICCx) + cos(wdt)*(romi_loc_y - ICCy) + ICCy;
        romi_loc_theta += wdt;

        if(romi_debug) {
        PRINT(R);
        PRINT(wdt * 180 / PI);
        }
    }
    romi_loc_theta = abs(romi_loc_theta) > 2 * PI ? romi_loc_theta - sign(romi_loc_theta) * 2 * PI : romi_loc_theta;

    if(romi_debug) {
        PRINT(left_motor_dist);
        PRINT(right_motor_dist);
        Serial.println(" ");
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
    RomiUpdateStateUsingICR(left_motor_dist, right_motor_dist);  
}

void RomiRotateLeft(float deg) {
    // Move the motors
    mcRotateLeft(deg);
    mcWaitDelayMoving(10);
    float left_motor_dist = mcGetDistanceLeft();
    float right_motor_dist = mcGetDistanceRight();
    RomiUpdateStateUsingICR(left_motor_dist, right_motor_dist);
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
    float target_angle = (atan2(dy, dx) - romi_loc_theta) * 180 / PI;
    
    Serial.print(target_dist);
    Serial.print(" ");
    Serial.println(target_angle);

    // Move Parameters
    RomiRotateLeft(target_angle);
    RomiMoveDistance(target_dist);
}

bool RomiGoHome(float x=0, float y=0) {
    Serial.println("Going Home!");
    // Go to specified home position
    RomiNavigateTo(x, y);

    // Straighten up
    RomiRotateLeft(-romi_loc_theta * PI / 180);

    Serial.println("HOME");
}


/* 

    Romi Motion Line Sensing

*/
bool RomiMoveForwardFindLine(float max_dist_millimeters) {
    bool foundLine = false;
    mcMoveDistance(max_dist_millimeters);
    while(mcIsMoving()) {
        if(lsensor.anyOnLine()) {
            mcStopMotors();
            RomiBuzzBuzzer(0);
            foundLine = true;
        }
        delay(10);
    }
    float left_motor_dist = mcGetDistanceLeftAndReset();
    float right_motor_dist = mcGetDistanceRightAndReset();
    RomiUpdateStateUsingICR(left_motor_dist, right_motor_dist);
    if(!foundLine){RomiBuzzBuzzer(3);}
    return foundLine;
}

void RomiFollowLine(bool endfind=true) {

    byte state_update_loop = 1;
    float target_vel = PI * wheel_radius / 2;
    mcSetVelocity(target_vel); // Set Constant Velocity

    byte state_update = 0;
    while (true) { // Update with end of line detection
        
        mcVelocityControlLoop();

        if (state_update >= state_update_loop) {
            float left_motor_dist = mcGetDistanceLeftAndReset();
            float right_motor_dist = mcGetDistanceRightAndReset();
            RomiUpdateStateUsingICR(left_motor_dist, right_motor_dist);
            state_update = 0;
        }
        state_update++;

        // Check if reached angular section and try to find line again
        if(!lsensor.anyOnLine() && endfind) {
            Serial.print("Reached CAP Searching...");
            RomiVelocityStop();

            float left_motor_dist = mcGetDistanceLeftAndReset();
            float right_motor_dist = mcGetDistanceRightAndReset();
            RomiUpdateStateUsingICR(left_motor_dist, right_motor_dist);

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

                // Update State due to rotation
                float left_motor_dist = mcGetDistanceLeftAndReset();
                float right_motor_dist = mcGetDistanceRightAndReset();
                RomiUpdateStateUsingICR(left_motor_dist, right_motor_dist);
                

                if (foundLine) {
                    Serial.println("Line refound");
                    break; // Continue following
                }
            }
            if(!foundLine) {
                Serial.println("End PATH");
                break; // End of Path
            }
        }


        float lsens_lr_error = lsensor.lineSensorError();
        float heading_ctrl = lsensor_heading_pid.update(lsens_lr_error);
        mcSetVelocityDelta(target_vel, heading_ctrl);        
        

        delay(8);
    }

    RomiVelocityStop();

    RomiBuzzBuzzer(0);
}

void RomiVelocityGo() {
    mcSetVelocity(PI * wheel_radius / 2);
    while(true) {
        mcVelocityControlLoop();
        float left_motor_dist = mcGetDistanceLeftAndReset();
        float right_motor_dist = mcGetDistanceRightAndReset();
        PRINT(left_motor_dist);
        PRINT(right_motor_dist);
        PRINT(left_motor_dist / right_motor_dist);
        Serial.println("");
        RomiUpdateStateUsingICR(left_motor_dist, right_motor_dist);
        delay(100);
    }
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
    Serial.println(romi_loc_theta * 180 / PI);
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

void __BuzzBuzzer(int duration, int del) {
    long start_time = millis();
    while(millis() - start_time < duration) {
        digitalWrite(Buzzer_Pin, HIGH);
        delay(del);
        digitalWrite(Buzzer_Pin, LOW);
        delay(del);
    }
}

void RomiBuzzBuzzer(int tune) {
    // switch(tune) {
    //     case 0: buzzer.play("!L16 V8 cdefgab>cbagfedc"); break;
    //     case 1: buzzer.play("!T240 L8 agafaea dac+adaea fa<aa<bac#a dac#adaea f4"); break;
    //     case 2: buzzer.play("!V8 c<a-<f");break; //failure
    //     case 3: buzzer.play("!L4 V8 dc");
    //     default: buzzer.play("c");
    // }
    switch(tune) {
        case 0: __BuzzBuzzer(500, 1); break;
        case 1: 
            __BuzzBuzzer(500, 2); 
            delay(100);
            __BuzzBuzzer(500, 2); break;
        case 2: 
            __BuzzBuzzer(500, 1); 
            __BuzzBuzzer(500, 3); 
            break;
        case 3:
            __BuzzBuzzer(500, 1);
            __BuzzBuzzer(500, 2);
            __BuzzBuzzer(500, 3);
            __BuzzBuzzer(500, 2);
            __BuzzBuzzer(500, 1);
            break;
        default:
            break;
    }
}



