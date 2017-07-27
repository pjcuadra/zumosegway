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
#include<ZumoControlledMotors.h>
#include<ZumoIMUFilters.h>
#include<Util.h>

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
   * Constructors
   * @param freq sampling frequency
   */
  ZumoSegway(double freq) {
    motors = new ZumoControlledMotors(freq);
    imu = new ZumoIMUFilters();

    this->freq = freq;

    // Connect actuator
    motors->speed = speed_s;

    // Connect sensors
    imu->angle_out = angle_s;
    imu->angular_speed_out = angular_speed_s;
  }

  /**
   *  Simulate the component
   */
  inline double simulate() {

    speed_s.write(speed.read());
    imu->simulate();


  }

  inline double motor() {

    angle.write(angle_s.read());
    angular_speed.write(angular_speed_s.read());

    // Set speed to 0 if angle is over 45 for safety
    if (abs(angle.read()) > DEG2RAD(45)) {
      speed_s.write(0.0);
      motors->simulate();
    }

    motors->simulate();

    return 0;

  }

private:
  /** Zumo motors */
  ZumoControlledMotors * motors;
  /** Zumo IMU */
  ZumoIMUFilters * imu;
  double freq;

  Signal angle_s;
  Signal angular_speed_s;
  Signal speed_s;

};

#endif
