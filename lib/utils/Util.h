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
#define _DEG2RAD (_PI / 180.0)

#define DEG2RAD(x) x * _DEG2RAD
#define RAD2DEG(x) x / _DEG2RAD

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

#endif
