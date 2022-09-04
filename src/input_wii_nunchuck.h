#pragma once
#include "input.h"
#include <WiiChuck.h>

//Represents in put from Will Cogley's 3-D printed eymech controller
// http://www.nilheim.co.uk/latest-projects-and-blog/eyemech-controller
class WiiNunchuckInput: public EyemechInput
{
 public:
	WiiNunchuckInput();
  void init();
	void update();
	int getEyeXPulse();
	int getEyeYPulse();
	bool getBlinking();
	int getLidUpperLeftPulse();
	int getLidUpperRightPulse();
	int getLidLowerLeftPulse();
	int getLidLowerRightPulse();

 private:
	Accessory nunchuck;
  int prevTrimVal;

	int eyeXPulse;
	int eyeYPulse;
	bool blinking;
	int lidUpperLeftPulse;
	int lidUpperRightPulse;
	int lidLowerLeftPulse;
	int lidLowerRightPulse;
};
