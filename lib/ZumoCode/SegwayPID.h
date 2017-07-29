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

#ifndef SEGWAYPID_H_
#define SEGWAYPID_H_

// Misc.
#include <ArduinoCode.h>
#include <Util.h>

// Debugging utils
#include <Plotter.h>

// Components
#include <ZumoSegway.h>
#include <PID.h>
#include <Adder.h>

class SegwayPID: public ArduinoCode {
public:

  void loop();

private:
  // Constants
  /** Sampling period in ms */
  const byte sampling_period = 20;
  /** Sampling frequency */
  const double sampling_freq = 1 / ((double) sampling_period / 1000.0);
  /** Initializing time */
  const double init_time = 2;
  /** Initializing Cycles */
  const int init_cycles = (init_time * 1000.0) / 20;

  // Components
  /** Segway Component */
  ZumoSegway * segway;
  /** Angle correction adder */
  Adder * angle_corrector;
  /** PID controller */
  PID * pid_c;

  // Signals
  /** Angle of the Segway with respect to the perpendicular to the floor */
  Signal angle;
  /** Angular speed of the Segway */
  Signal angular_speed;
  /** Linear speed of the Segway */
  Signal speed;
  /** Combined angle from info out of gyro and accelerometer */
  Signal combined_angle;
  /** Corrected Angle because of the deviation of the center of gravity */
  Signal corrected_angle;
  /** Negative of the angle of the center of gravity */
  Signal center_angle;

  // Other variables
  /** Serial received character */
  char c = ' ';
  /** Current state */
  zumo_states_e curr_state = S_INITIALIZING;

  /** Initializing cycles init_cycle_count */
  int init_cycle_count;

  /**
   * Simluate the entire circuit
   */
  void simulate_circuit();

  /**
   * Build the circuit connecting all components together
   */
  void build_circuit();

  void setup();


};

#endif
