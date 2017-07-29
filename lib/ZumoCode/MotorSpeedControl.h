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

#ifndef MOTORSPEEDCONTROL_H_
#define MOTORSPEEDCONTROL_H_

// Misc.
#include <ArduinoCode.h>
#include <Util.h>

// Debugging utils
#include <Plotter.h>

// Components
#include <ZumoSegway.h>
#include <StateFeedback.h>
#include <Adder.h>

// Components
#include <ZumoControlledMotors.h>

class MotorSpeedControl: public ArduinoCode {
public:
  void loop();
  void setup();

  MotorSpeedControl() : ArduinoCode() {
    // Do nothing for now
  }

private:
  // Constants
  /** Sampling period in ms */
  const byte sampling_period = 20;
  /** Sampling frequency */
  const double sampling_freq = 1 / ((double) sampling_period / 1000.0);

  // Needed for starting balancing
  Zumo32U4ButtonA buttonA;
  Zumo32U4Encoders encoders;

  // Components
  /** Segway Component */
  ZumoControlledMotors * motors;

  // Signals
  Signal speed;

  // Other variables
  /** Last sampled time */
  byte last_sampled_time = 0;

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
