#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

const int MAGNET_PINS[] = {3, 4, 5};
const int MAGNET_COUNT = sizeof(MAGNET_PINS) / sizeof(MAGNET_PINS[0]);

const int PWM_FREQUENCY_HZ = 50;
const int PWM_MIN = 150;
const int PWM_MAX = 600;
const int SERVO_RESET_LOW = 225;
const int SERVO_RESET_HIGH = 525;

const int STEP_DELAY_MS = 10;
const int MOTION_SETTLE_DELAY_MS = 5000;

const char COMMAND_FORWARD = '0';
const char COMMAND_LEFT = '1';
const char COMMAND_RIGHT = '2';

struct MotionStep {
  double startA;
  double startB;
  double endA;
  double endB;
  double stepA;
  double stepB;
};

const MotionStep MOTION_STEPS[] = {
  {30.0, 150.0, 9.155, 108.31, -0.1, -0.2},
  {9.155, 108.31, 108.31, 9.155, 0.1, -0.1},
  {108.31, 9.155, 150.0, 30.0, 0.2, 0.1},
};

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(PWM_FREQUENCY_HZ);

  for (int i = 0; i < MAGNET_COUNT; i++) {
    pinMode(MAGNET_PINS[i], OUTPUT);
  }
}

void loop() {
  enableAllMagnets();

  if (!Serial.available()) {
    return;
  }

  char command = Serial.read();
  if (command == COMMAND_FORWARD) {
    Serial.print(command);
    act(0, 1);
  } else if (command == COMMAND_LEFT) {
    Serial.print(command);
    act(2, 0);
  } else if (command == COMMAND_RIGHT) {
    Serial.print(command);
    act(1, 2);
  }
}

void enableAllMagnets() {
  for (int i = 0; i < MAGNET_COUNT; i++) {
    digitalWrite(MAGNET_PINS[i], HIGH);
  }
}

void act(int servoA, int servoB) {
  for (int i = 0; i < 3; i++) {
    moveServoPair(servoA, servoB, MOTION_STEPS[i]);
  }

  pwm.setPWM(servoA, 0, SERVO_RESET_LOW);
  pwm.setPWM(servoB, 0, SERVO_RESET_HIGH);
  delay(MOTION_SETTLE_DELAY_MS);
}

void moveServoPair(int servoA, int servoB, MotionStep step) {
  double angleA = step.startA;
  double angleB = step.startB;

  while (isWithinRange(angleA, step.endA, step.stepA) &&
         isWithinRange(angleB, step.endB, step.stepB)) {
    pwm.setPWM(servoA, 0, angleToPulse(angleA));
    pwm.setPWM(servoB, 0, angleToPulse(angleB));

    angleA += step.stepA;
    angleB += step.stepB;
    delay(STEP_DELAY_MS);
  }
}

bool isWithinRange(double value, double endValue, double stepValue) {
  if (stepValue > 0) {
    return value <= endValue;
  }
  return value >= endValue;
}

int angleToPulse(double angle) {
  return constrain(map(angle, 0, 180, PWM_MIN, PWM_MAX), PWM_MIN, PWM_MAX);
}
