#pragma once

// Abstract class for getting eyemech input
class EyemechInput
{
 public:
  virtual void init() = 0;
	virtual void update() = 0;

	virtual int getEyeXPulse() = 0;
	virtual int getEyeYPulse() = 0;
	virtual bool getBlinking() = 0;
	virtual int getLidUpperLeftPulse() = 0;
	virtual int getLidUpperRightPulse() = 0 ;
	virtual int getLidLowerLeftPulse() = 0;
	virtual int getLidLowerRightPulse() = 0;
};
