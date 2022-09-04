#pragma once

//Represents in put from Will Cogley's 3-D printed eymech controller
// http://www.nilheim.co.uk/latest-projects-and-blog/eyemech-controller
class EyemechControllerInput: public EyemechInput
{
 public:
	EyemechControllerInput(int pinJoystickX_, int pinJoystickY_, int pinBlinkButton_, int pinEyelidTrim_);
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
	int pinJoystickX;
	int pinJoystickY;
	int pinBlinkButton;
	int pinEyelidTrim;

	int eyeXPulse;
	int eyeYPulse;
	bool blinking;
	int lidUpperLeftPulse;
	int lidUpperRightPulse;
	int lidLowerLeftPulse;
	int lidLowerRightPulse;
};
