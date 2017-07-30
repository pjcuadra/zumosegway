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

#ifndef UTIL_H_
#define UTIL_H_

#ifdef ARDUINO
#include <Arduino.h>
#endif

#define _PI 3.1416

#define DEG2RAD(x) (float)(x * (float)(_PI / 180.0))
#define RAD2DEG(x) (float)(x * (float)(180.0 / _PI))

inline void startup_wait() {
  // Needed for starting balancing
  Zumo32U4ButtonA buttonA;
  // Display the angle until the user presses A.
  while (!buttonA.getSingleDebouncedRelease()) {
    // Just wait ;)
    ledRed(true);
  }
  ledRed(false);
}

inline void startup_wait(Serial_ * serial) {
  // Needed for starting balancing
  Zumo32U4ButtonA buttonA;
  // Display the angle until the user presses A.
  while (!buttonA.getSingleDebouncedRelease()) {
    // Just wait ;)
    ledRed(true);

    if (serial->available()) {
      if ('s' == serial->read()) {
        break;
      }
    }
  }
  ledRed(false);
}

inline void setup_gyro(L3G &gyro) {
  // Set up the L3GD20H gyro.
  gyro.init();

  // 800 Hz output data rate,
  // low-pass filter cutoff 100 Hz.
  gyro.writeReg(L3G::CTRL1, 0b11111111);

  // 2000 dps full scale.
  gyro.writeReg(L3G::CTRL4, 0b00100000);

  // High-pass filter disabled.
  gyro.writeReg(L3G::CTRL5, 0b00000000);
}

inline void digital_filter(const int degree, const float *a, const float *b, float *x, float *y, float in, float &out) {

  float tmp_out = in * b[0];

  // Update the state
  for (int i = 0; i < degree; i++){
    tmp_out += b[i + 1] * x[i] - a[i + 1] * y[i];
  }

  // Shift states
  for (int i = degree - 1; i > 0; i--){
    x[i] = x[i - 1];
    y[i] = y[i - 1];
  }

  x[0] = in;
  y[0] = tmp_out/a[0];

  out = y[0];
}

inline void setup_accelerometer(LSM303 &compass) {
  // Set up the LSM303D accelerometer.
  compass.init();

  // 50 Hz output data rate
  compass.writeReg(LSM303::CTRL1, 0x57);

  // 8 g full-scale
  compass.writeReg(LSM303::CTRL2, 0x18);
}


#endif
