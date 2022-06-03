//  (Modified) Nilheim Mechatronics Simplified Eye Mechanism Code

//  X-axis joystick pin: A1
//  Y-axis joystick pin: A0
//  Trim potentiometer pin: A2
//  Button pin: 2
#include <Arduino.h>
#include "basic-eyemech.h"

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define PIN_JOYSTICK_X A1
#define PIN_JOYSTICK_Y A0
#define PIN_EYELID_TRIM A2
#define PIN_BLINK_BUTTON 2

#define SERVO_EYE_X 0
#define SERVO_EYE_Y 1
#define SERVO_LID_UPPER_LEFT 2
#define SERVO_LID_LOWER_LEFT 3
#define SERVO_LID_UPPER_RIGHT 4
#define SERVO_LID_LOWER_RIGHT 5

#define SERVOMIN  140 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  520 // this is the 'maximum' pulse length count (out of 4096)

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

int xval;
int yval;

int eyeXPulse;
//int rexpulse;

int eyeYPulse;
//int reypulse;

int lidUpperLeftPulse;
int lidLowerLeftPulse;
int lidUpperRightPulse;
int lidLowerRightPulse;

int trimval;

int sensorValue = 0;
int outputValue = 0;
int switchval = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("8 channel Servo test!");
  pinMode(PIN_JOYSTICK_Y, INPUT);
  pinMode(2, INPUT);

  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  delay(10);
}

// you can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. its not precise!
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;

  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz
  Serial.print(pulselength); Serial.println(" us per period");
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit");
  pulse *= 1000000;  // convert to us
  pulse /= pulselength;
  Serial.println(pulse);

}

void loop() {

  xval = analogRead(PIN_JOYSTICK_X);
  eyeXPulse = map(xval, 0, 1023, 220, 440);
  //rexpulse = lexpulse;

  switchval = digitalRead(PIN_BLINK_BUTTON);

  yval = analogRead(PIN_JOYSTICK_Y);
  eyeYPulse = map(yval, 0, 1023, 250, 500);
  //reypulse = map(yval, 0, 1023, 400, 280);

  trimval = analogRead(PIN_EYELID_TRIM);
  trimval = map(trimval, 320, 580, -40, 40);
  lidUpperLeftPulse = map(yval, 0, 1023, 400, 280);
  lidUpperLeftPulse -= (trimval - 40);
  lidUpperLeftPulse = constrain(lidUpperLeftPulse, 280, 400);
  lidUpperRightPulse = 680 - lidUpperLeftPulse;

  lidLowerLeftPulse = map(yval, 0, 1023, 410, 280);
  lidLowerLeftPulse += (trimval / 2);
  lidLowerLeftPulse = constrain(lidLowerLeftPulse, 280, 400);
  lidLowerRightPulse = 680 - lidLowerLeftPulse;

  // Set servo values
  pwm.setPWM(SERVO_EYE_X, 0, eyeXPulse);
  pwm.setPWM(SERVO_EYE_Y, 0, eyeYPulse);

  if (switchval == HIGH) {
    pwm.setPWM(SERVO_LID_UPPER_LEFT, 0, 400);
    pwm.setPWM(SERVO_LID_LOWER_LEFT, 0, 240);
    pwm.setPWM(SERVO_LID_UPPER_RIGHT, 0, 240);
    pwm.setPWM(SERVO_LID_LOWER_RIGHT, 0, 400);
  } else if (switchval == LOW) {
    pwm.setPWM(SERVO_LID_UPPER_LEFT, 0, lidUpperLeftPulse);
    pwm.setPWM(SERVO_LID_LOWER_LEFT, 0, lidLowerLeftPulse);
    pwm.setPWM(SERVO_LID_UPPER_RIGHT, 0, lidUpperRightPulse);
    pwm.setPWM(SERVO_LID_LOWER_RIGHT, 0, lidLowerRightPulse);
  }

  Serial.println(trimval);
  delay(5);
}
