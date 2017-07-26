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

#ifndef ZUMOMOTORS_H_
#define ZUMOMOTORS_H_

#ifdef ARDUINO
#include <Arduino.h>
#include <Zumo32U4.h>
#endif

#include<Component.h>

/**
 * Signal Class
 */
class ZumoMotors: public Component {
public:
  /** Left speed in port */
  Port left_speed;
  /** Right speed in port */
  Port right_speed;
  /** Scale left speed */
  double left_scale;
  /** Scale right speed */
  double right_scale;
  /** Dead zone range */
  int dead_zone;

  /**
   *  Constructors
   */
  ZumoMotors() {
    // Set params to defualts
    left_scale = 1;
    right_scale = 1;
    dead_zone = 0;
  }

  /**
   *  Simulate the component
   */
  inline double simulate() {
    double left = left_scale * left_speed.read();
    double right = right_scale * right_speed.read();

    const int dead_zone_shift = 0;

    // Correct the deadzone
    if (abs(left) < dead_zone + dead_zone_shift) {
      if (left < 0) {
        left -= dead_zone + dead_zone_shift;
      }
      if (left > 0) {
        left += dead_zone + dead_zone_shift;
      }
    }

    // Correct the deadzone
    if (abs(right) < dead_zone) {
      if (right < 0) {
        right -= dead_zone;
      }
      if (right > 0) {
        right += dead_zone;
      }
    }



    // Set scale, corrected and rotating speeds
    motors.setSpeeds(constrain(left, -400, 400), constrain(right, -400, 400));
    return 0;
  }

private:
  /** Zumo motors */
  Zumo32U4Motors motors;

};

#endif
