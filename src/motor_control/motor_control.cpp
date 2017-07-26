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

#ifndef UNIT_TEST

#ifdef ARDUINO
#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#endif

// Misc.
#include <Util.h>

// Debugging utils
#include <Plotter.h>

// Components
#include <ZumoControlledMotors.h>

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
/** Serial Plotter */
Plotter * plotter;

// Functions prototypes
void simulate_circuit();
void build_circuit();

/**
 * Setup function
 */
void setup() {

  Wire.begin();
  delay(500);
  Serial.begin(115200);
  plotter = new Plotter(&Serial);
  delay(1000);

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


  // Build the circuit connecting all components together
  build_circuit();

  // Display the angle until the user presses A.
  while (!buttonA.getSingleDebouncedRelease()) {
    // Just wait ;)
    ledRed(true);

    if (Serial1.available()) {
      if ('s' == Serial1.read()) {
        break;
      }
    }
  }
  ledRed(false);

  speed = 2 * _PI;
  encoders.getCountsAndResetLeft();

}

/**
 * Loop function
 */
void loop() {

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
void simulate_circuit() {
  motors->simulate();

  plotter->plot(2, motors->speed_out.read());
  plotter->plot(3, motors->speed.read());

}

/**
 * Build the circuit connecting all components together
 */
void build_circuit() {
  // Connect Plant's inputs
  motors->speed = speed;
}

#endif
