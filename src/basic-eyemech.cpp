//  (Modified) Nilheim Mechatronics Simplified Eye Mechanism Code

#include <Arduino.h>
#include "basic-eyemech.h"

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "input.h"

//#define INPUT_METHOD_EYEMECHCONTROLLER 1;
#define INPUT_METHOD_WIINUNCHUCK 1;

#ifdef INPUT_METHOD_EYEMECHCONTROLLER
#include "input_eyemech_controller.h"

#define PIN_JOYSTICK_X A1
#define PIN_JOYSTICK_Y A0
#define PIN_EYELID_TRIM A2
#define PIN_BLINK_BUTTON 4

EyemechControllerInput input = EyemechControllerInput(PIN_JOYSTICK_X, PIN_JOYSTICK_Y, PIN_BLINK_BUTTON, PIN_EYELID_TRIM);
#endif

#ifdef INPUT_METHOD_WIINUNCHUCK
#include "input_wii_nunchuck.h"

WiiNunchuckInput input = WiiNunchuckInput();
#endif

// Also, SCL and SDA are used by pwm

#define SERVO_EYE_X 0
#define SERVO_EYE_Y 1
#define SERVO_LID_UPPER_LEFT 2
#define SERVO_LID_LOWER_LEFT 3
#define SERVO_LID_UPPER_RIGHT 4
#define SERVO_LID_LOWER_RIGHT 5

#define SERVOMIN  205 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  410 // this is the 'maximum' pulse length count (out of 4096)

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup() {
  Serial.begin(9600);

  pwm.begin();
  pwm.setPWMFreq(50);  // Analog servos run at ~60 Hz updates // TODO: should be 50hz akshually
	input.init();
}

void loop() {
	input.update();

	// Set servo values
  pwm.setPWM(SERVO_EYE_X, 0, input.getEyeXPulse());
  pwm.setPWM(SERVO_EYE_Y, 0, input.getEyeYPulse());

  if (input.getBlinking() ) {
    pwm.setPWM(SERVO_LID_UPPER_LEFT, 0, 400);
    pwm.setPWM(SERVO_LID_LOWER_LEFT, 0, 240);
    pwm.setPWM(SERVO_LID_UPPER_RIGHT, 0, 240);
    pwm.setPWM(SERVO_LID_LOWER_RIGHT, 0, 400);
  } else {
    pwm.setPWM(SERVO_LID_UPPER_LEFT, 0, input.getLidUpperLeftPulse());
    pwm.setPWM(SERVO_LID_LOWER_LEFT, 0, input.getLidLowerLeftPulse());
    pwm.setPWM(SERVO_LID_UPPER_RIGHT, 0, input.getLidUpperRightPulse());
    pwm.setPWM(SERVO_LID_LOWER_RIGHT, 0, input.getLidLowerRightPulse());
  }

  delay(5);
}
