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
#ifdef NNNN
#ifndef UNIT_TEST

#ifdef ARDUINO
#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#endif

// Misc.
#include <Util.h>

// Debugging utils
#include <MotorSpeedControl.h>

/**
 * Setup function
 */
inline void MotorSpeedControl::setup() {

  plotter->config_plot(0, "title:{Current Time}");

  // Create zumo components
  motors = new ZumoControlledMotors(sampling_freq);

  // Print contantas
  plotter->info("Sampling Frequency", sampling_freq);

  // Configure the plots
  plotter->config_plot(0, "title:{Current Time}");
  plotter->config_plot(0, "period:{20}");

  plotter->config_plot(1, "title:{Sampling Period}");
  plotter->config_plot(1, "period:{20}");

  plotter->config_plot(2, "title:{Speed}");

  speed = 2 * _PI;
  encoders.getCountsAndResetLeft();

}

/**
 * Loop function
 */
inline void MotorSpeedControl::loop() {

    // Sampling period is sampling_period
  byte current_time = millis();
  if ((byte)(current_time - last_sampled_time) >= sampling_period) {
    // Current Time
    plotter->plot(0, current_time);

    // Sampling period
    plotter->plot(1, (byte)(current_time - last_sampled_time));

    last_sampled_time = current_time;
    simulate_circuit();
  }
}

/**
 * Simluate the entire circuit
 */
void MotorSpeedControl::simulate_circuit() {
  float tmp = motors->simulate(speed);

  plotter->plot(2, tmp);
  plotter->plot(3, speed);
}

#endif
#endif
