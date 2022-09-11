//  (Modified) Nilheim Mechatronics Simplified Eye Mechanism Code

#include <Arduino.h>
#include "basic-eyemech.h"

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "input.h"
#include "animation.h"

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

#define PIN_MODE_SWITCH 10

AnimationInput animator = AnimationInput();
// Also, SCL and SDA are used by pwm

#define SERVO_EYE_X 0
#define SERVO_EYE_Y 1
#define SERVO_LID_UPPER_LEFT 2
#define SERVO_LID_LOWER_LEFT 3
#define SERVO_LID_UPPER_RIGHT 4
#define SERVO_LID_LOWER_RIGHT 5

#define SERVOMIN  205 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  410 // this is the 'maximum' pulse length count (out of 4096)


enum InputMode {
	InputMode_Controller,
	InputMode_Animation
};

InputMode mode = InputMode_Controller;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup() {
  Serial.begin(9600);

	randomSeed(analogRead(0));

  pwm.begin();
  pwm.setPWMFreq(50);  // Analog servos run at ~60 Hz updates // TODO: should be 50hz akshually
	input.init();

	pinMode(PIN_MODE_SWITCH, INPUT);
}

void loop() {
  InputMode prevMode = mode;
	mode = digitalRead(PIN_MODE_SWITCH) ? InputMode_Animation : InputMode_Controller;

	if (mode == InputMode_Animation && mode != prevMode) {
		animator.init();
	}

	EyemechInput *source;
	switch (mode) {
	case InputMode_Animation:
		source = &animator;
		break;
	case InputMode_Controller:
		source = &input;
		break;
	}

	source->update();

	// Set servo values
  pwm.setPWM(SERVO_EYE_X, 0, source->getEyeXPulse());
  pwm.setPWM(SERVO_EYE_Y, 0, source->getEyeYPulse());

  if (source->getBlinking() ) {
		pwm.setPWM(SERVO_LID_UPPER_LEFT, 0, 240);
    pwm.setPWM(SERVO_LID_LOWER_LEFT, 0, 400);
    pwm.setPWM(SERVO_LID_UPPER_RIGHT, 0, 400);
    pwm.setPWM(SERVO_LID_LOWER_RIGHT, 0, 240);
  } else {
		pwm.setPWM(SERVO_LID_UPPER_LEFT, 0, source->getLidUpperLeftPulse());
    pwm.setPWM(SERVO_LID_LOWER_LEFT, 0, source->getLidLowerLeftPulse());
    pwm.setPWM(SERVO_LID_UPPER_RIGHT, 0, source->getLidUpperRightPulse());
    pwm.setPWM(SERVO_LID_LOWER_RIGHT, 0, source->getLidLowerRightPulse());
  }

  delay(5);
}
