#pragma once
#include "input.h"
#include <Arduino.h>

struct AnimationTarget {
  int duration;

  int eyeXPulse;
  int eyeYPulse;
  int lidTrim;
  bool blinking;
};

#define ANIMATION_MAX_SEQUENCE_SIZE                                            \
  3 // this should be the max  # of targets added ot the animation buffer at one
    // time by generateTargets

#define ANIMATION_BUFFER_SIZE (ANIMATION_MAX_SEQUENCE_SIZE * 2)
#define ANIMATION_BUFFER_NEXT(x) (((x) + 1) % ANIMATION_BUFFER_SIZE)
#define ANIMATION_BUFFER_PREV(x)                                               \
  (((x) + ANIMATION_BUFFER_SIZE - 1) % ANIMATION_BUFFER_SIZE)
#define ANIMATION_BUFFER_DIST(from, to)                                        \
  ((from) <= (to) ? (to) - (from) : ANIMATION_BUFFER_SIZE - (to) + (from))

#define SERVO_PULSE_MIN  205 // this is the 'minimum' pulse length count (out of 4096)
#define SERVO_PULSE_MAX  410 // this is the 'maximum' pulse length count (out of 4096)


#define BLINK_DURATION 500

// Represents in put from Will Cogley's 3-D printed eymech controller
//  http://www.nilheim.co.uk/latest-projects-and-blog/eyemech-controller
class AnimationInput : public EyemechInput {
public:
  AnimationInput();
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
  byte index;
  byte writeIndex;
  AnimationTarget targets[ANIMATION_BUFFER_SIZE];
  unsigned long lastMillis;

  int eyeXPulse;
  int eyeYPulse;
  bool blinking;
  int lidUpperLeftPulse;
  int lidUpperRightPulse;
  int lidLowerLeftPulse;
  int lidLowerRightPulse;

  void generateTargets();
  void generateRandomDirection(int *x, int *y);
};
