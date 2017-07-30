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
  /**
   *  Simulate the component
   */
  inline float set_speed(float speed) {
    // Set scale, corrected and rotating speeds
    motors.setSpeeds(constrain(speed, -400, 400), constrain(speed, -400, 400));
    return speed;
  }

private:
  /** Zumo motors */
  Zumo32U4Motors motors;
};

#endif
