/*###################
 * Romi test script
 * ##################
 * 
 * This script is designed to test that your Romi is functioning to at least a minimal level. 
 * Upon uploading the script, you should see:
 * 
 *    1. An orange LED flashing at 5 Hz for 5 seconds
 *    2. A short tone played by the Buzzer
 *    3. The left wheel spins in one direction
 *    4. The left wheel spins in the opposite direction
 *    5. The right wheel spins in one direction
 *    6. The right wheel spins in the opposite direction
 *    7. Two short tones are played by the Buzzer
 *    
 * If you do not see this sequence, please note the failure on the checklist, post the error on the blackboard forum (As much information as possible please), and
 * either bring the robot to a health clinic, or arrange a time to return it to either Paul or Martin.
 */

unsigned long t0; //Timing variable
const byte BUZZER_PIN = 6;
const byte LEFT_SPEED_PIN = 10;
const byte LEFT_DIR_PIN = 16;
const byte RIGHT_SPEED_PIN = 9;
const byte RIGHT_DIR_PIN = 15;

void setup() 
{
  //Setup pins
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LEFT_SPEED_PIN, OUTPUT);
  pinMode(LEFT_DIR_PIN, OUTPUT);
  pinMode(RIGHT_SPEED_PIN, OUTPUT);
  pinMode(RIGHT_DIR_PIN, OUTPUT);

  
  t0 = millis();
  
}

void loop() 
{

  //Flash LEDs
  for (int i=0;i<50;i++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }

  //Play tone
  analogWrite(BUZZER_PIN, 127);
  delay(500);
  digitalWrite(BUZZER_PIN, LOW);

  //Spin motors
  //Left first
  digitalWrite(LEFT_DIR_PIN, HIGH);
  spin_up_down(LEFT_SPEED_PIN);
  digitalWrite(LEFT_DIR_PIN, LOW);
  spin_up_down(LEFT_SPEED_PIN);
  //Then right
  digitalWrite(RIGHT_DIR_PIN, HIGH);
  spin_up_down(RIGHT_SPEED_PIN);
  digitalWrite(RIGHT_DIR_PIN, LOW);
  spin_up_down(RIGHT_SPEED_PIN);

    //Play tone
  analogWrite(BUZZER_PIN, 127);
  delay(500);
  digitalWrite(BUZZER_PIN, LOW);

    //Play tone
  analogWrite(BUZZER_PIN, 127);
  delay(500);
  digitalWrite(BUZZER_PIN, LOW);

  while(true)
  {
    //Script is finished -- block forever
  }
 
}

void spin_up_down(byte speed_pin)
{
  for (int i=0;i<100;i=i+10)
  {
    analogWrite(speed_pin, i);
    delay(500);
  }

  for (int i=100;i>0;i=i-10)
  {
    analogWrite(speed_pin, i);
    delay(500);
  }

  analogWrite(speed_pin, 0);
}

