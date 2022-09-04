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

#define SERVOMIN  140 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  520 // this is the 'maximum' pulse length count (out of 4096)

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup() {
  Serial.begin(9600);
  Serial.println("8 channel Servo test!");

  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
	input.init();

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
