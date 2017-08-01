/* Copyright 2017 Pedro Cuadra & Meghadoot Gardi
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at

 *  http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */
#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;

/** Sampling Period in ms */
const uint8_t samplingPeriodMS = 20;
/** Sampling Period in s */
const float samplingPeriod = samplingPeriodMS / 1000.0;
/** Sampling frequency */
const float samplingFrequency = 1/samplingPeriod;

/** Low pass filter angular Position*/
float angularPositionLP = 0;
/** Zumo's angular position */
float angularPosition = 0;
/** Zumo's angular speed */
float angularSpeed = 0;

/** PWM signal applied to the motor's driver 400 is 100% cicle and -400 100% but inverse direction */
int32_t speed;
/** Error of the angle */
float angleError = 0;

/**
 * Setup Function
 */
void setup() {
  Wire.begin();

  Serial.begin(115200);

  // Setup the IMU
  setupIMU();

  // Calibrate the IMU (obtain the offset)
  calibrateGyro();

  // Display the angle until the user presses A.
  while (!buttonA.getSingleDebouncedRelease()) {
    // Update the angle using the gyro as often as possible.
    sampleGyro();

    // Every 20 ms (50 Hz), correct the angle using the
    // accelerometer and also print it.
    static uint8_t lastCorrectionTime = 0;
    uint8_t m = millis();
    if ((uint8_t)(m - lastCorrectionTime) >= samplingPeriodMS)
    {
      lastCorrectionTime = m;
      sampleAccelerometer();
    }
  }
  delay(500);
}

/**
 * Main loop Function
 */
void loop() {
  // Update the angle using the gyro as often as possible.
  sampleGyro();

  // Every 20 ms (50 Hz), correct the angle using the
  // accelerometer, print it, and set the motor speeds.
  static byte lastCorrectionTime = 0;
  byte m = millis();
  if ((byte)(m - lastCorrectionTime) >= 20)
  {
    lastCorrectionTime = m;
    sampleAccelerometer();
    filterAngle(); // Low pass filter (IRR)
    setActuators();
  }
}

/**
 * Apply IRR low pass filter to the angular position
 */
void filterAngle() {
  const int degree = 3;
  const float a[degree + 1] = {1.000000, -0.577241, 0.421787, -0.056297};
  const float b[degree + 1] = {0.098531, 0.295593, 0.295593, 0.098531};
  static float x[degree + 1] = {0, 0, 0, 0};
  static float y[degree + 1] = {0, 0, 0, 0};

  digitalFilter(degree, a, b, x, y, angularPosition, angularPositionLP);
}

/**
 * Control the actuators
 */
void setActuators() {
  const float targetAngle = -0.4;

  angularPosition = angularPositionLP;
    
  if (abs(angularPosition) > 45) {
    // If the robot is tilted more than 45 degrees, it is
    // probably going to fall over.  Stop the motors to prevent
    // it from running away.
    speed = 0;
  } else {
    angleError = targetAngle - angularPosition;
    pid();
    speed = constrain(speed, -400, 400);
  }
  motors.setSpeeds(speed, speed);

  Serial.println(angularPosition);
}

