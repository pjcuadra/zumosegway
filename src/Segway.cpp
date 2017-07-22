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

// Include debugging utils
#include <Plotter.h>

// Include components
#include <ZumoSegway.h>
#include <StateFeedback.h>
#include <Adder.h>

// Needed for starting balancing
Zumo32U4ButtonA buttonA;

// Components
ZumoSegway * segway;
StateFeedback<2, 1> * control_law;
Adder * error_calc;

// Signals
Signal angle;
Signal angular_speed;
Signal speed;
Signal error;
Signal target_angle;

// Controll Law
double K[2] = {0.72944, -2.18832};

// Other variables
byte sampling_time = 0;
const byte sampling_period = 20;

// Functions prototypes
void simulate_circuit();
void build_circuit();

// Controller buttoms
const byte B_LEFT    = 48;
const byte B_UP      = 49;
const byte B_RIGHT   = 50;
const byte B_DOWN    = 51;
const byte B_SELECT  = 52;
const byte B_START   = 53;
const byte B_SQUARE  = 54;
const byte B_TRIAGLE = 55;
const byte B_CROSS   = 56;
const byte B_CIRCLE  = 57;

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

/** Initializing Cycles */
const int init_cycles = 100;
/** Serial received character */
char c = ' ';
/** Calibrated Target Angle */

/** Current state */
zumo_states_e curr_state = S_INITIALIZING;
/** Initializing cycles init_cycle_count */
int init_cycle_count;
/** Serial Plotter */
Plotter * plotter;

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
  segway = new ZumoSegway();
  control_law = new StateFeedback<2, 1>(K);
  error_calc = new Adder();

  target_angle = -2;

  plotter->info("K[0]", K[0]);
  plotter->info("K[1]", K[1]);
  plotter->info("Target Angle", target_angle.read());

  // Configure the plots
  plotter->config_plot(0, "title:{Current Time}");
  plotter->config_plot(0, "period:{20}");

  plotter->config_plot(1, "title:{Sampling Period}");
  plotter->config_plot(1, "period:{20}");

  plotter->config_plot(2, "title:{Angle}");
  plotter->config_plot(2, "period:{20}");

  plotter->config_plot(3, "title:{Speed}");
  plotter->config_plot(3, "period:{20}");

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

  // Init the counter
  init_cycle_count = 0;

  plotter->info("Initializing State");
  curr_state = S_INITIALIZING;
}

/**
 * Loop function
 */
void loop() {

  if (Serial1.available()) {
    c = Serial1.read();
  } else {
    c = ' ';
  }

  switch (curr_state) {
    case S_INITIALIZING:
      // Change state once the amount of cycles have been reached
      if (init_cycle_count > init_cycles) {
        curr_state = S_BALANCING;
        plotter->info("Balacing State");
        speed = 0;
      }

      break;
    case S_BALANCING:
      // Update the target angle
      ledYellow(false);

      switch (c) {
        case B_SELECT: // Start calibrating state
          curr_state = S_CALIBRATING;
          plotter->info("Calibration State");
          break;
        case B_UP: // Start moving foward
          curr_state = S_MOVING_FORWARD;
          plotter->info("Moving Forward State");
          break;
        case B_DOWN: // Start moving backwards
          curr_state = S_MOVING_BACKWARDS;
          plotter->info("Moving Backwards State");
          break;
        case B_TRIAGLE:
          curr_state = S_BALANCING;
          plotter->info("Balancing State");
          break;
        case B_CROSS:
          curr_state = S_BALANCING;
          plotter->info("Balancing State");
          break;
        default:
          break;
      }


      break;
    case S_CALIBRATING:
      ledYellow(true);
      switch (c) {
        case B_SELECT: // Exit calibrating state
          curr_state = S_BALANCING;
          break;
        default:
          break;
      }
      break;
    case S_MOVING_FORWARD:
      if (c == B_DOWN) {
        curr_state = S_BALANCING;
      }
    case S_MOVING_BACKWARDS:
      if (c == B_UP) {
        curr_state = S_BALANCING;
      }
      if (c == B_START) {
        curr_state = S_BALANCING;
      }
      ledYellow(false);
      break;
    default:
      ledYellow(false);
      break;
  }

    // Sampling period is sampling_period
  byte current_time = millis();
  if ((byte)(current_time - sampling_time) >= sampling_period) {
    // Current Time
    plotter->plot(0, current_time);

    // Sampling period
    plotter->plot(1, (byte)(current_time - sampling_time));

    sampling_time = current_time;
    simulate_circuit();
  }
}

/**
 * Simluate the entire circuit
 */
void simulate_circuit() {
  if (curr_state == S_INITIALIZING) {
    init_cycle_count++;
    return;
  }

  // IMU simulation doesn't depend on angle
  error_calc->simulate();
  segway->simulate();
  control_law->simulate();

  plotter->plot(2, segway->angle.read());
  plotter->plot(3, segway->speed.read());
}

/**
 * Build the circuit connecting all components together
 */
void build_circuit() {
  // Connect Plant's inputs
  segway->angle = angle;
  segway->angular_speed = angular_speed;

  // Connect Control Law input
  control_law->in[0] = angle;
  control_law->in[1] = angular_speed;

  // Connect Control Law output
  control_law->out[0] = speed;

  // Connect the error calculator
  error_calc->in_0 = speed;
  error_calc->in_1 = target_angle;
  error_calc->out = error;

  // Connect Plant's output
  segway->speed = error;
}

#endif
