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

#ifdef SEGWAY

#ifndef UNIT_TEST

#ifdef ARDUINO
#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#endif

// Misc.
#include <Commands.h>
#include <Util.h>

// Debugging utils
#include <Plotter.h>

// Components
#include <ZumoSegway.h>
#include <StateFeedback.h>
#include <Adder.h>

// Constants
/** Controll Law */
double K[2] = {-364.70, -287.38};
/** Sampling period in ms */
const byte sampling_period = 20;
/** Sampling frequency */
const double sampling_freq = 1 / ((double) sampling_period / 1000.0);
/** Initializing Cycles */
const int init_cycles = 100;

// Needed for starting balancing
Zumo32U4ButtonA buttonA;

// Components
/** Segway Component */
ZumoSegway * segway;
/** Full-state feedback component */
StateFeedback<2, 1> * control_law;
/** Angle correction adder */
Adder * angle_corrector;
/** Integrator to convert to speed */
Integral * integrator;

// Signals
/** Angle of the Segway with respect to the perpendicular to the floor */
Signal angle;
/** Angular speed of the Segway */
Signal angular_speed;
/** Linear speed of the Segway */
Signal speed;
/** Linear acceleration of the Segway */
Signal acceleration;
/** Corrected Angle because of the deviation of the center of gravity */
Signal corrected_angle;
/** Negative of the angle of the center of gravity */
Signal center_angle;

// Other variables
/** Last sampled time */
byte last_sampled_time = 0;
/** Serial received character */
char c = ' ';
/** Current state */
zumo_states_e curr_state = S_INITIALIZING;
/** Initializing cycles init_cycle_count */
int init_cycle_count;
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
  const float a = 1;
  K[0] = a*K[0];
  K[1] = a*K[1];

  // Create zumo components
  segway = new ZumoSegway();
  control_law = new StateFeedback<2, 1>(K);
  angle_corrector = new Adder();
  integrator = new Integral(-400, 400, sampling_freq);

  center_angle = DEG2RAD(-2);

  plotter->info("K[0]", K[0]);
  plotter->info("K[1]", K[1]);
  plotter->info("Target Angle", center_angle.read());
  plotter->info("Sampling Frequency", sampling_freq);

  // Configure the plots
  plotter->config_plot(0, "title:{Current Time}");
  plotter->config_plot(0, "period:{20}");

  plotter->config_plot(1, "title:{Sampling Period}");
  plotter->config_plot(1, "period:{20}");

  plotter->config_plot(2, "title:{Angle}");
  plotter->config_plot(2, "period:{20}");

  plotter->config_plot(3, "title:{Angular Speed}");
  plotter->config_plot(3, "period:{20}");

  plotter->config_plot(3, "title:{Linear Acceleration}");
  plotter->config_plot(3, "period:{20}");

  plotter->config_plot(4, "title:{Speed}");
  plotter->config_plot(4, "period:{20}");

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
  if (curr_state == S_INITIALIZING) {
    init_cycle_count++;
    return;
  }

  // IMU simulation doesn't depend on angle
  segway->simulate();
  //angle = 5;
  // angular_speed = 0;
  angle_corrector->simulate();
  control_law->simulate();
  integrator->simulate();
  segway->motor();

  plotter->plot(2, RAD2DEG(corrected_angle.read()));
  plotter->plot(3, RAD2DEG(segway->angular_speed.read()));
  plotter->plot(4, acceleration.read());
  plotter->plot(5, speed.read());
}

/**
 * Build the circuit connecting all components together
 */
void build_circuit() {
  // Connect Plant's inputs
  segway->angle = angle;
  segway->angular_speed = angular_speed;

  // Connect the error calculator
  angle_corrector->in_0 = angle;
  angle_corrector->in_1 = center_angle;
  angle_corrector->out = corrected_angle;

  // Connect Control Law input
  control_law->in[0] = corrected_angle;
  control_law->in[1] = angular_speed;

  // Connect Control Law output
  control_law->out[0] = acceleration;

  integrator->in = acceleration;
  integrator->out = speed;

  // Connect Plant's output
  segway->speed = speed;
}

#endif
#endif
