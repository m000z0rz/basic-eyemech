#include <Arduino.h>
#include "input.h"
#include "input_wii_nunchuck.h"
#include <WiiChuck.h>

WiiNunchuckInput::WiiNunchuckInput() {};

void WiiNunchuckInput::init()
{
	nunchuck.begin();

	nunchuck.type = NUNCHUCK; // this line shouldn't be necessary
  if (nunchuck.type == Unknown){
		nunchuck.type = NUNCHUCK;
	}

	prevTrimVal = 0;
  delay(10);
}

void WiiNunchuckInput::update()
{
  nunchuck.readData();
  //nunchuck.printInputs();

  int xval = nunchuck.getJoyX();
  eyeXPulse = map(xval, 0, 255, 220, 440);

  int yval = nunchuck.getJoyY();
  eyeYPulse = map(yval, 0, 255, 250, 500);

  blinking = nunchuck.getButtonC();

	int trimval = 0;
  if (nunchuck.getButtonZ()) {
    trimval = nunchuck.getRollAngle();
    trimval = map(trimval, 50, -50, -40, 40);
  } else {
    trimval = prevTrimVal;
  }

  lidUpperLeftPulse = map(yval, 0, 255, 400, 280);
  lidUpperLeftPulse -= (trimval - 40);
  lidUpperLeftPulse = constrain(lidUpperLeftPulse, 280, 400);
  lidUpperRightPulse = 680 - lidUpperLeftPulse;

  lidLowerLeftPulse = map(yval, 0, 255, 410, 280);
  lidLowerLeftPulse += (trimval / 2);
  lidLowerLeftPulse = constrain(lidLowerLeftPulse, 280, 400);
  lidLowerRightPulse = 680 - lidLowerLeftPulse;

  prevTrimVal = trimval;
}

int WiiNunchuckInput::getEyeXPulse() { return eyeXPulse; }
int WiiNunchuckInput::getEyeYPulse() { return eyeYPulse; }
bool WiiNunchuckInput::getBlinking() { return blinking; }
int WiiNunchuckInput::getLidUpperLeftPulse() { return lidUpperLeftPulse; }
int WiiNunchuckInput::getLidUpperRightPulse() { return lidUpperRightPulse; }
int WiiNunchuckInput::getLidLowerLeftPulse() { return lidLowerLeftPulse; }
int WiiNunchuckInput::getLidLowerRightPulse() { return lidLowerRightPulse; }
