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

#ifndef IMU_H_
#define IMU_H_

// Misc.
#include <ArduinoCode.h>
#include <Util.h>

// Debugging utils
#include <Plotter.h>

// Components
#include <ZumoIMU.h>
#include <ZumoIMUFilters.h>

class IMU: public ArduinoCode {
public:

  void loop();

  void setup();

  IMU() : ArduinoCode() {
    // Do nothing
  }

private:
  ZumoIMUFilters * imu_filters;
  ZumoIMU * imu;
  Signal imu_angle;
  Signal imu_filters_angle;
  Signal imu_filters_angular_speed;
  Signal imu_filters_combined_angle;
  // Needed for starting balancing
  Zumo32U4ButtonA buttonA;

  /**
   * Simluate the entire circuit
   */
  void simulate_circuit();

  /**
   * Build the circuit connecting all components together
   */
  void build_circuit();


};

#endif
