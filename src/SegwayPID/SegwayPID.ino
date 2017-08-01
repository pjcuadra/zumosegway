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

float freq = 50;

float P = 40;
float I = 576;
float D = 0.5625;

float a = (P + I/(2 * freq) + D * freq);
float b = (-P + I/(2 * freq) - 2 * freq * D);
float c = D * freq;

float lp_a_angle = 0;
float angle = 0;

void setup() {
  Wire.begin();

  Serial.begin(115200);

  setup_gyro();
  setup_accelerometer();

  gyro_calibrate();

  // Display the angle until the user presses A.
  while (!buttonA.getSingleDebouncedRelease()) {
    // Update the angle using the gyro as often as possible.
    updateAngleGyro();

    // Every 20 ms (50 Hz), correct the angle using the
    // accelerometer and also print it.
    static uint8_t lastCorrectionTime = 0;
    uint8_t m = millis();
    if ((uint8_t)(m - lastCorrectionTime) >= 20)
    {
      lastCorrectionTime = m;
      correctAngleAccel();
    }
  }
  delay(500);
}

void loop() {
  // Update the angle using the gyro as often as possible.
  updateAngleGyro();

  // Every 20 ms (50 Hz), correct the angle using the
  // accelerometer, print it, and set the motor speeds.
  static byte lastCorrectionTime = 0;
  byte m = millis();
  if ((byte)(m - lastCorrectionTime) >= 20)
  {
    lastCorrectionTime = m;
    correctAngleAccel();
    // filter_angular_speed();
    filter_angle();
    // get_combined_angle();
    setMotors();
  }
}

// Low pass filter for the output
void filter_angle() {
  const int degree = 3;
  const float a[4] = {1.000000, -0.577241, 0.421787, -0.056297};
  const float b[4] = {0.098531, 0.295593, 0.295593, 0.098531};
  static float x[4] = {0, 0, 0, 0};
  static float y[4] = {0, 0, 0, 0};

  digital_filter(degree, a, b, x, y, angle, lp_a_angle);
}

// This function uses our current angle estimation and a PID
// algorithm to set the motor speeds.  This is the core of the
// robot's balancing algorithm.
void setMotors() {
  const float targetAngle = -0.4;

  angle = lp_a_angle;

  int32_t speed;
  if (abs(angle) > 45) {
    // If the robot is tilted more than 45 degrees, it is
    // probably going to fall over.  Stop the motors to prevent
    // it from running away.
    speed = 0;
  } else {

    static float u_1 = 0;
    static float e_1 = 0;
    static float e_2 = 0;

    float error = targetAngle - angle;

    speed = u_1 + a * error + b * e_1 + c * e_2;
    speed = constrain(speed, -400, 400);

    e_2 = e_1;
    e_1 = error;
    u_1 = speed;

  }
  motors.setSpeeds(speed, speed);

  Serial.print("1 ");
  Serial.println(angle);
  //
  // Serial.print("2 ");
  // Serial.println(a_angle);
  //
  // Serial.print("3 ");
  // Serial.println(lp_a_angle);
}

