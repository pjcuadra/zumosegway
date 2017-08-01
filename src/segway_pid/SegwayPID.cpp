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
#ifndef UNIT_TEST

// Misc.
#include <SegwayPID.h>
#include <Commands.h>
#include <Integral.h>

void SegwayPID::setup() {
  Wire.begin();

  Serial.begin(115200);

  setup_gyro(gyro);
  setup_accelerometer(compass);

  a_angle = 0;

  ledYellow(1);

  // Delay to give the user time to remove their finger.
  delay(500);

  // Calibrate the gyro.
  for (uint16_t i = 0; i < 1024; i++) {
    // Wait for new data to be available, then read it.
    while(!gyro.readReg(L3G::STATUS_REG) & 0x08);
    gyro.read();

    // Add the Y axis reading to the total.
    gyroOffsetY += gyro.g.y;
  }
  gyroOffsetY /= 1024;

  ledYellow(0);

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

void SegwayPID::loop() {
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

void SegwayPID::get_angular_speed() {
  gyro.read();
  g_read = -gyro.g.y;

  g_angular_speed = g_read * 70 / 1000000;
}

void SegwayPID::filter_angular_speed() {
  const int degree = 3;
  const float a[4] = {1.00000, -2.37409, 1.92936, -0.53208};
  const float b[4] = {0.72944, -2.18832, 2.18832, -0.72944};
  static float x[4] = {0, 0, 0, 0};
  static float y[4] = {0, 0, 0, 0};

  float tmp_out = g_angular_speed * b[0];

  // Update the state
  for (int i = 0; i < degree; i++){
    tmp_out += b[i + 1] * x[i] - a[i + 1] * y[i];
  }

  // Shift states
  for (int i = degree - 1; i > 0; i--){
    x[i] = x[i - 1];
    y[i] = y[i - 1];
  }

  x[0] = g_angular_speed;
  y[0] = tmp_out/a[0];

  hp_g_angular_speed = y[0];
}

// Reads the gyro and uses it to update the angle estimation.
void SegwayPID::updateAngleGyro() {
  // Figure out how much time has passed since the last update.
  // Figure out how much time has passed since the last update.
  static uint16_t lastUpdate = 0;
  uint16_t m = micros();
  uint16_t dt = m - lastUpdate;
  lastUpdate = m;

  get_angular_speed();

  // Calculate how much the angle has changed, in degrees, and
  // add it to our estimation of the current angle.  The gyro's
  // sensitivity is 0.07 dps per digit.
  angle += ((float)gyroOffsetY + g_read) * 70 * dt / 1000000000;
}

inline void SegwayPID::get_angle() {
  compass.read();
  a_angle = atan2(compass.a.z, -compass.a.x) * 180 / M_PI;
}


// Low pass filter for the output
void SegwayPID::filter_angle() {
  const int degree = 3;
  const float a[4] = {1.000000, -0.577241, 0.421787, -0.056297};
  const float b[4] = {0.098531, 0.295593, 0.295593, 0.098531};
  static float x[4] = {0, 0, 0, 0};
  static float y[4] = {0, 0, 0, 0};

  digital_filter(degree, a, b, x, y, angle, lp_a_angle);
}

inline void SegwayPID::get_combined_angle() {
  static float x_1 = 0;
  static float y_1 = 0;

  float tmp = y_1 + (hp_g_angular_speed + x_1) / (2 *  freq);

  /*
   * Discrete integrator
   * y(k) = y(k - 1) + (x(k) + x(k - 1)) * Ts / 2
   */

  // Update stored states
  y_1 = constrain(tmp, -180, 180);
  x_1 = hp_g_angular_speed;

  c_angle = tmp + lp_a_angle;

}

// Reads the accelerometer and uses it to adjust the angle
// estimation.
void SegwayPID::correctAngleAccel() {
  get_angle();

  // Calculate the angle according to the accelerometer.
  aAngle = a_angle;

  // Calculate the magnitude of the measured acceleration vector,
  // in units of g.
  LSM303::vector<float> const aInG = {
    (float)compass.a.x / 4096,
    (float)compass.a.y / 4096,
    (float)compass.a.z / 4096}
  ;
  float mag = sqrt(LSM303::vector_dot(&aInG, &aInG));

  // Calculate how much weight we should give to the
  // accelerometer reading.  When the magnitude is not close to
  // 1 g, we trust it less because it is being influenced by
  // non-gravity accelerations, so we give it a lower weight.
  float weight = 1 - 5 * abs(1 - mag);
  weight = constrain(weight, 0, 1);
  weight /= 10;

  // Adjust the angle estimation.  The higher the weight, the
  // more the angle gets adjusted.
  angle = weight * aAngle + (1 - weight) * angle;
}

// This function uses our current angle estimation and a PID
// algorithm to set the motor speeds.  This is the core of the
// robot's balancing algorithm.
void SegwayPID::setMotors() {
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

#endif
