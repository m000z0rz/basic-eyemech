#include "animation.h"
#include <Arduino.h>

AnimationInput::AnimationInput() {};

void AnimationInput::init() {
  index = 0;
  writeIndex = 0;
  lastMillis = millis();
}

// uint8_t AnimationInput::bufferDistance(uint8_t from, uint8_t to) {
//   return (from <= to) ? to - from
// 		: (sizeof(targets) / sizeof(AnimationTarget)) - to + from;
// }

void AnimationInput::update() {
  byte bufferDepth = ANIMATION_BUFFER_DIST(index, writeIndex);

	// if we don't have a current and next and next-next, then generate mroe
	// eye movement plans
	// next and next-next are needed in case we advance index and need to ease
	// into the following one (easing is currently not implemented)
  if (bufferDepth < 3) {
    generateTargets();
  }

  unsigned long dt = millis() - lastMillis;
  if (dt > (24l * 60 * 60 * 1000)) {
		// if greater than a day, surely we wrapped
		// around? just ignore this iteration

		dt = 0;
	}

  int duration = targets[index].duration - dt;
  if (duration <= 0) {
    // Advance to next target
		index = ANIMATION_BUFFER_NEXT(index);
    duration += targets[index].duration;
	}
	targets[index].duration = duration;

  AnimationTarget target = targets[index];
  eyeXPulse = target.eyeXPulse;
  eyeYPulse = target.eyeYPulse;
  blinking = target.blinking;

  int trim = target.lidTrim;
  lidUpperLeftPulse = map(eyeYPulse, 250, 500, 400, 280);
  lidUpperLeftPulse -= (trim - 40);
  lidUpperLeftPulse = constrain(lidUpperLeftPulse, 280, 400);
  lidUpperRightPulse = 680 - lidUpperLeftPulse;

  lidLowerLeftPulse = map(eyeYPulse, 250, 500, 410, 280);
  lidLowerLeftPulse += (trim / 2);
  lidLowerLeftPulse = constrain(lidLowerLeftPulse, 280, 400);
  lidLowerRightPulse = 680 - lidLowerLeftPulse;

  lastMillis = millis();
}

void AnimationInput::generateTargets() {
  if (ANIMATION_BUFFER_SIZE - ANIMATION_BUFFER_DIST(index, writeIndex) <
      ANIMATION_MAX_SEQUENCE_SIZE) {
    Serial.println(
									 "!! Not enough room in buffer to reliably generate new targets!");
  }

	int prevIndex = ANIMATION_BUFFER_PREV(writeIndex); // keep this 1 behind writeIndex
	while(ANIMATION_BUFFER_SIZE - ANIMATION_BUFFER_DIST(index, writeIndex) >= ANIMATION_MAX_SEQUENCE_SIZE) {

		int rand = random(0, 100); // 0-99
		if (rand < 20) {
			// blink and keep looking same direction
			targets[writeIndex].duration = BLINK_DURATION;
			targets[writeIndex].eyeXPulse = targets[prevIndex].eyeXPulse;
			targets[writeIndex].eyeYPulse = targets[prevIndex].eyeYPulse;
			targets[writeIndex].blinking = true;
			targets[writeIndex].lidTrim = targets[prevIndex].lidTrim;

			writeIndex = ANIMATION_BUFFER_NEXT(writeIndex);
			prevIndex = ANIMATION_BUFFER_NEXT(prevIndex);

			targets[writeIndex].duration = random(250, 5000);
			targets[writeIndex].eyeXPulse = targets[prevIndex].eyeXPulse;
			targets[writeIndex].eyeYPulse = targets[prevIndex].eyeYPulse;
			targets[writeIndex].blinking = false;
			targets[writeIndex].lidTrim = targets[prevIndex].lidTrim;

			writeIndex = ANIMATION_BUFFER_NEXT(writeIndex);
			prevIndex = ANIMATION_BUFFER_NEXT(prevIndex);
		}
		else if (rand < 25) {

			// get shifty-eyed
			targets[writeIndex].duration = random(3000, 7000);
			if (random(0, 100) < 50) {
				targets[writeIndex].eyeXPulse = targets[prevIndex].eyeXPulse;
				targets[writeIndex].eyeYPulse = targets[prevIndex].eyeYPulse;
			}
			else {
				generateRandomDirection(
																&(targets[writeIndex].eyeXPulse),
																&(targets[writeIndex].eyeYPulse)
																);
			}
			targets[writeIndex].blinking = false;
			targets[writeIndex].lidTrim = -40; // TODO: or 40?

			writeIndex = ANIMATION_BUFFER_NEXT(writeIndex);
			prevIndex = ANIMATION_BUFFER_NEXT(prevIndex);
		}
		else if (rand < 60){
			// move
			generateRandomDirection(
															&(targets[writeIndex].eyeXPulse),
															&(targets[writeIndex].eyeYPulse)
															);
			targets[writeIndex].duration = random(250, 3000);
			targets[writeIndex].blinking = false;
			targets[writeIndex].lidTrim = targets[prevIndex].lidTrim;

			writeIndex = ANIMATION_BUFFER_NEXT(writeIndex);
			prevIndex = ANIMATION_BUFFER_NEXT(prevIndex);
		}
		else {
			// move and blink (and remove squint)
			generateRandomDirection(
															&(targets[writeIndex].eyeXPulse),
															&(targets[writeIndex].eyeYPulse)
															);
			targets[writeIndex].duration = BLINK_DURATION;
			targets[writeIndex].blinking = true;
			targets[writeIndex].lidTrim = 0;

			writeIndex = ANIMATION_BUFFER_NEXT(writeIndex);
			prevIndex = ANIMATION_BUFFER_NEXT(prevIndex);

			targets[writeIndex].eyeXPulse = targets[prevIndex].eyeXPulse;
			targets[writeIndex].eyeYPulse = targets[prevIndex].eyeYPulse;
			targets[writeIndex].blinking = false;
			targets[writeIndex].duration = random(250, 5000);
			targets[writeIndex].lidTrim = 0;

			writeIndex = ANIMATION_BUFFER_NEXT(writeIndex);
			prevIndex = ANIMATION_BUFFER_NEXT(prevIndex);
		}
	}
}

void AnimationInput::generateRandomDirection(int *x, int *y)
{
	float angle = 2 * PI * (random(0, 1024) / 1024.f);
	double r = sqrt(sqrt(random(0, 32768))) / 32768.;
	// now angle is in radians and r is from 0-1, weighted towards center
	double magnitude = (r * (SERVO_PULSE_MAX - SERVO_PULSE_MIN)) + SERVO_PULSE_MIN;
	*x = magnitude * cos(angle);
	*y = magnitude * sin(angle);
}

int AnimationInput::getEyeXPulse() { return eyeXPulse; }
int AnimationInput::getEyeYPulse() { return eyeYPulse; }
bool AnimationInput::getBlinking() { return blinking; }
int AnimationInput::getLidUpperLeftPulse() { return lidUpperLeftPulse; }
int AnimationInput::getLidUpperRightPulse() { return lidUpperRightPulse; }
int AnimationInput::getLidLowerLeftPulse() { return lidLowerLeftPulse; }
int AnimationInput::getLidLowerRightPulse() { return lidLowerRightPulse; }
