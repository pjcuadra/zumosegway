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
    // Just wait
  }
  delay(500);
}

void SegwayLQR::loop() {
  // Every 20 ms (50 Hz), correct the angle using the
  // accelerometer, print it, and set the motor speeds.
  static byte lastCorrectionTime = 0;
  byte m = millis();
  if ((byte)(m - lastCorrectionTime) >= 20)
  {
    lastCorrectionTime = m;
    get_angle();
    get_angular_speed();
    get_omega();
    compensator();
    setMotors();
  }
}

// TODO: Add HPF here
void SegwayLQR::get_angular_speed() {
  gyro.read();
  g_read = -gyro.g.y;

  g_angular_speed = DEG2RAD(((float)gyroOffsetY + g_read) * 70 / 1000000);
}

void SegwayLQR::filter_angular_speed() {
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

inline void SegwayLQR::get_angle() {
  static float a_angle_1;
  compass.read();
  a_angle = atan2(compass.a.z, -compass.a.x);

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
  a_angle_1 = weight * a_angle + (1 - weight) * a_angle_1;

  a_angle = a_angle_1;
}


// Low pass filter for the output
void SegwayLQR::filter_angle() {
  const int degree = 3;
  const float a[4] = {1.000000, -0.577241, 0.421787, -0.056297};
  const float b[4] = {0.098531, 0.295593, 0.295593, 0.098531};
  static float x[4] = {0, 0, 0, 0};
  static float y[4] = {0, 0, 0, 0};

  digital_filter(degree, a, b, x, y, angle, lp_a_angle);
}

inline void SegwayLQR::get_combined_angle() {
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

void SegwayLQR::get_omega() {
  float left_speed = encoders.getCountsAndResetLeft() * 2 * _PI * freq * count2cycle;
  float right_speed = encoders.getCountsAndResetRight() * 2 * _PI * freq * count2cycle;

  // Estimate the omega out of the average of both motors' speeds
  e_omega = (left_speed + right_speed) / 2;

}

void SegwayLQR::compensator() {
  const float targetAngle = 0;
  static float o_x[states] = {0, 0, 0, 0, 0, 0};
  static float o_x_dot[states] = {0, 0, 0, 0, 0, 0};

  static float y[outputs] = {0, 0, 0};
  static float y_tilde[outputs] = {0, 0, 0};


  // Form y vector
  y[0] = a_angle - targetAngle;
  y[1] = g_angular_speed;
  y[2] = e_omega;

  // Calculate y_tilde
  for (int o = 0; o < outputs; o++) {
    y_tilde[o] = y[o];
    for (int s = 0; s < states; s++) {
        y_tilde[o] -= C[o][s]*o_x[s];
    }
  }

  // Calculate state derivative
  for (int s = 0; s < states; s++) {
    o_x_dot[s] = B[s] * u;

    // Multiply with A matrix
    for (int s_1 = 0; s_1 < states; s_1++) {
      o_x_dot[s] += A[s][s_1]*o_x[s_1];
    }

    // Multiply by L matrix
    for (int o = 0; o < outputs; o++) {
      o_x_dot[s] += L[s][o]*y[o];
    }
  }

  u = 0;

  // Calculate new states
  for (int s = 0; s < states; s++) {
    // Calc new state
    o_x[s] = o_x_dot[s]/freq + o_x[s];

    // Calc the output
    u += o_x[s]*K[s];
  }

}

// This function uses our current angle estimation and a PID
// algorithm to set the motor speeds.  This is the core of the
// robot's balancing algorithm.
void SegwayLQR::setMotors() {


  angle = a_angle;

  int32_t speed;
  if (abs(RAD2DEG(angle)) > 45) {
    // If the robot is tilted more than 45 degrees, it is
    // probably going to fall over.  Stop the motors to prevent
    // it from running away.
    speed = 0;
  } else {

    compensator();

    speed = constrain(u, -400, 400);

  }

  motors.setSpeeds(speed, speed);

  Serial.print("1 ");
  Serial.println(a_angle);
  //
  // Serial.print("2 ");
  // Serial.println(g_angular_speed);
  //
  // Serial.print("3 ");
  // Serial.println(e_omega);
}

#endif
