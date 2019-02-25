#include "PIDController.h"

/*
 * Class constructor
 * This runs whenever we create an instance of the class
 */
 PID::PID(float P, float D, float I)
{
  //Store the gains
  setGains(P, D, I);
  //Set last_millis
  last_millis = 0;
}

/*
 * This function prints the individual contributions to the total contol signal
 * You can call this yourself for debugging purposes, or set the debug flag to true to have it called
 * whenever the update function is called.
 */
void PID::print_components()
{
  Serial.print(" P: ");
  Serial.print(Kp_output);
  Serial.print(" D: ");
  Serial.print(Kd_output);
  Serial.print(" I: ");
  Serial.print(Ki_output);
  Serial.print(" Total: ");
  Serial.println(total);
}

/*
 * This function sets the gains of the PID controller
 */
void PID::setGains(float P, float D, float I)
{
  Kp = P;
  Kd = D;
  Ki = I;
}

/*
 * This is the update function. 
 * This function should be called repeatedly. 
 * It takes a measurement of a particular variable (ex. Position, speed, heading) and a desired value for that quantity as input
 * It returns an output; this can be sent directly to the motors, 
 * combined with other control outputs
 * or sent as input to another controller
 */
float PID::update(float error)
{
  //Calculate how much time (in milliseconds) has passed since the last update call
  long time_now = millis();
  int time_delta = time_now - last_millis;
  last_millis = time_now;

  /*
   * ================================
   * Your code goes implementation of a PID controller should go here
   * ================================
   */

  //This represents the error term
  // float error = demand - measurement;

  //This represents the error derivative
  float error_delta = (error - last_error) / time_delta;
  last_error = error;
  last_delta = error_delta;

  //Update storage
  integral_error = integral_error + error;

  /*
   * ===========================
   * Code below this point should not need to be changed
   */

  //Calculate components
  Kp_output = Kp*error;
  Kd_output = Kd*error_delta;
  Ki_output = Ki*integral_error;

  //Add the three components to get the total output
  total = Kp_output + Kd_output + Ki_output;

  //Make sure we don't exceed the maximum output
  total = constrain(total, -max_output, max_output);

  //Print debugging information if required
  if (debug)
  {
    Serial.print("Error: ");
    Serial.print(error);
    Serial.print(" Delta: ");
    Serial.print(error_delta);
    print_components();
  }
  
  return total;
}

bool PID::settled(float error) {
  long time_now = millis();
  int time_delta = time_now - last_millis;
  // float error = demand - measurement;
  float error_delta = (error - last_error) / time_delta;
  return (error_delta - last_delta) == 0;
}

void PID::reset() {
  integral_error = 0;
  last_delta = 0;
  last_error = 0;
}

void PID::setMax(float newMax)
{
  if (newMax > 0)
  {
    max_output = newMax;
  }
  else
  {
    Serial.println("Max output must be positive");
  }
}

void PID::setDebug(bool state)
{
  debug = state;
}