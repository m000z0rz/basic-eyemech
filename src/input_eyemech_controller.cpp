#include <Arduino.h>
#include "input.h"
#include "input_eyemech_controller.h"

EyemechControllerInput::EyemechControllerInput(int pinJoystickX_, int pinJoystickY_, int pinBlinkButton_, int pinEyelidTrim_)
{
	pinJoystickX = pinJoystickX_;
	pinJoystickY = pinJoystickY_;
	pinBlinkButton = pinBlinkButton_;
	pinEyelidTrim = pinEyelidTrim_;
};

void EyemechControllerInput::init()
{
	pinMode(pinJoystickX, INPUT);
	pinMode(pinJoystickY, INPUT);
	pinMode(pinBlinkButton, INPUT);
	pinMode(pinEyelidTrim, INPUT);
}

void EyemechControllerInput::update() {
  int xval = analogRead(pinJoystickX);
  map(xval, 0, 1023, 220, 440);

  int yval = analogRead(pinJoystickY);
  map(yval, 0, 1023, 250, 500);

  blinking = digitalRead(pinBlinkButton) == HIGH;

  int trimval = analogRead(pinEyelidTrim);
  trimval = map(trimval, 320, 580, -40, 40);

  lidUpperLeftPulse = map(yval, 0, 1023, 400, 280);
  lidUpperLeftPulse -= (trimval - 40);
  lidUpperLeftPulse = constrain(lidUpperLeftPulse, 280, 400);
  lidUpperRightPulse = 680 - lidUpperLeftPulse;

  lidLowerLeftPulse = map(yval, 0, 1023, 410, 280);
  lidLowerLeftPulse += (trimval / 2);
  lidLowerLeftPulse = constrain(lidLowerLeftPulse, 280, 400);
  lidLowerRightPulse = 680 - lidLowerLeftPulse;
}


int EyemechControllerInput::getEyeXPulse() { return eyeXPulse; }
int EyemechControllerInput::getEyeYPulse() { return eyeYPulse; }
bool EyemechControllerInput::getBlinking() { return blinking; }
int EyemechControllerInput::getLidUpperLeftPulse() { return lidUpperLeftPulse; }
int EyemechControllerInput::getLidUpperRightPulse() { return lidUpperRightPulse; }
int EyemechControllerInput::getLidLowerLeftPulse() { return lidLowerLeftPulse; }
int EyemechControllerInput::getLidLowerRightPulse() { return lidLowerRightPulse; }
