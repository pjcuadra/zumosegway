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

#ifndef ZUMOSEGWAY_H_
#define ZUMOSEGWAY_H_

#ifdef ARDUINO
#include <Arduino.h>
#include <Zumo32U4.h>
#endif

#include<Component.h>
#include<ZumoMotors.h>
#include<ZumoIMUFilters.h>

/**
 * Zumo board states
 */
enum zumo_states_e {
  /** Initializing state */
  S_INITIALIZING,
  /** Balancing state */
  S_BALANCING,
  /** Calibrating state */
  S_CALIBRATING,
  /** Initializing state */
  S_MOVING_FORWARD,
  /** Initializing state */
  S_MOVING_BACKWARDS,
};

/**
 * Signal Class
 */
class ZumoSegway: public Component {
public:
  /** Left speed in port */
  Port speed;
  /** Angle out port */
  Port angle;
  /** Angular Speed out port */
  Port angular_speed;

  /**
   *  Constructors
   */
  ZumoSegway() {
    motors = new ZumoMotors();
    imu = new ZumoIMUFilters();

    // Connect actuator
    motors->left_speed = speed;
    motors->right_speed = speed;
    motors->dead_zone = 45;

    // Connect sensors
    imu->angle_out = angle;
    imu->angular_speed_out = angular_speed;
  }

  /**
   *  Simulate the component
   */
  inline double simulate() {

    motors->simulate();

    // Set speed to 0 if angle is over 45 for safety
    if (abs(angle.read()) > 45) {
      speed.write(0.0);
      return 0;
    }

    imu->simulate();

    return 0;
  }

private:
  /** Zumo motors */
  ZumoMotors * motors;
  /** Zumo IMU */
  ZumoIMUFilters * imu;
};

#endif
