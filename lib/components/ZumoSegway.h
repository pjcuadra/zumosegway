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
  /** Laying State */
  S_LAYING,
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
  /** Combined Angle */
  Port combined_angle;

  /**
   * Constructors
   * @param freq sampling frequency
   */
  ZumoSegway(double freq) {
    motors = new ZumoMotors();
    imu = new ZumoIMUFilters(-1000, 1000, freq);

    this->freq = freq;

    // Connect actuator
    motors->left_speed = speed_s;
    motors->right_speed = speed_s;

    // Connect sensors
    imu->angle_out = angle_s;
    imu->angular_speed_out = angular_speed_s;
    imu->combined_angle_out = combined_angle_s;
  }

  /**
   *  Simulate the component
   */
  inline double simulate() {

    speed_s.write(speed.read());
    imu->simulate();

    angle.write(angle_s.read());
    angular_speed.write(angular_speed_s.read());
    combined_angle.write(combined_angle_s.read());

    return 0;

  }

  inline double motor() {
    motors->simulate();
    return 0;
  }

private:
  /** Zumo motors */
  ZumoMotors * motors;
  /** Zumo IMU */
  ZumoIMUFilters * imu;
  double freq;

  Signal angle_s;
  Signal angular_speed_s;
  Signal combined_angle_s;
  Signal speed_s;

};

#endif
