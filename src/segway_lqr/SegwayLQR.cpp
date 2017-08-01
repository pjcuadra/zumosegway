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
#include <SegwayLQR.h>
#include <Commands.h>
#include <Integral.h>

void SegwayLQR::setup() {
  Wire.begin();

  Serial.begin(115200);

  setup_gyro(gyro);
  setup_accelerometer(compass);

  a_angular_position = 0;

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
    // While waiting let's give some time to settle
    get_zumo_angular_speed();
    get_zumo_angular_position();
  }
  delay(500);

  // Clear counters
  get_motor_angular_speed();
  e_angular_speed = 0;

}

void SegwayLQR::loop() {

  get_zumo_angular_speed();

  // Every 20 ms (50 Hz), correct the angle using the
  // accelerometer, print it, and set the motor speeds.
  static byte lastCorrectionTime = 0;
  byte m = millis();
  if ((byte)(m - lastCorrectionTime) >= 20)
  {
    lastCorrectionTime = m;
    // Motor angular speed first because we need to integrate
    x[2] = get_motor_angular_speed();
    x[0] = get_motor_angular_position();
    x[1] = get_zumo_angular_position(); // Corrected position
    x[3] = get_zumo_angular_speed();
    setMotors();
  }
}

// TODO: Add HPF here
inline float SegwayLQR::get_zumo_angular_speed() {
  // Figure out how much time has passed since the last update.
  static uint16_t lastUpdate = 0;
  uint16_t m = micros();
  uint16_t dt = m - lastUpdate;
  lastUpdate = m;

  gyro.read();

  g_angular_speed = ((float)gyroOffsetY - gyro.g.y) * 70 / 1000000;

  // Calculate how much the angle has changed, in degrees, and
  // add it to our estimation of the current angle.  The gyro's
  // sensitivity is 0.07 dps per digit.
  a_angular_position += g_angular_speed * dt / 1000;

  return g_angular_speed;
}

inline float SegwayLQR::get_zumo_angular_position() {
  static float a_angle_1;
  compass.read();
  a_angular_position = atan2(compass.a.z, -compass.a.x) * 180 / _PI;

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
  a_angle_1 = weight * a_angular_position + (1 - weight) * a_angle_1;

  a_angular_position = a_angle_1;

  return a_angular_position - target_angle;
}

inline float SegwayLQR::get_motor_angular_speed() {
  float left_speed = encoders.getCountsAndResetLeft() * 2 * _PI * freq * count2cycle;
  float right_speed = encoders.getCountsAndResetRight() * 2 * _PI * freq * count2cycle;

  // Estimate the omega out of the average of both motors' speeds
  e_angular_speed = (left_speed + right_speed) / 2;

  return e_angular_speed;
}

inline float SegwayLQR::get_motor_angular_position() {

  // Integrate speed
  e_angular_position += e_angular_speed / freq;

  return e_angular_position;
}

// This function uses our current angle estimation and a PID
// algorithm to set the motor speeds.  This is the core of the
// robot's balancing algorithm.
void SegwayLQR::setMotors() {

  angle = a_angular_position;

  int32_t speed = 0;
  if (abs(angle) > 45) {
    // If the robot is tilted more than 45 degrees, it is
    // probably going to fall over.  Stop the motors to prevent
    // it from running away.
    speed = 0;
  } else {
    for (int s = 0; s < states; s++) {
        speed -= scale*x[s]*K[s];
    }

    speed = constrain(speed, -400, 400);

  }

  motors.setSpeeds(speed, speed);

  // Serial.print("1 ");
  // Serial.println(RAD2DEG(x[0]));
  // Serial.print("2 ");
  // Serial.println(RAD2DEG(x[1]));
  // Serial.print("3 ");
  // Serial.println(RAD2DEG(x[2]));
  // Serial.print("4 ");
  // Serial.println(RAD2DEG(x[3]));
  //
}

#endif
