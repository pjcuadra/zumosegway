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

// Misc.
#include <SegwayPID.h>
#include <Commands.h>

Zumo32U4Buzzer buzzer;

/**
 * Setup function
 */
inline void SegwayPID::setup() {
  // Create zumo components
  segway = new ZumoSegway(sampling_freq);
  angle_corrector = new Adder(-1, 1);
  pid_c =  new PID(500, 0, 0, sampling_freq, -400, 400);

  center_angle = DEG2RAD(2);

  plotter->info("Target Angle", center_angle.read());
  plotter->info("Sampling Frequency", sampling_freq);

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

  // Wait for buttom release
  startup_wait();

  // Init the counter
  init_cycle_count = 0;

  plotter->info("Initializing State");
  curr_state = S_INITIALIZING;
}


inline void SegwayPID::loop() {

  switch (curr_state) {
    case S_INITIALIZING:
      // Change state once the amount of cycles have been reached
      if (init_cycle_count > init_cycles) {
        curr_state = S_BALANCING;
        plotter->info("Balacing State");
        speed = 0;
      }

      // Detect if angle is out of bounds
      if (abs(angle.read()) > DEG2RAD(45)) {
        plotter->info("Laying State");
        curr_state =  S_LAYING;
      }

      break;
    case S_BALANCING:
      // Detect if angle is out of bounds
      if (abs(angle.read()) > DEG2RAD(45)) {
        plotter->info("Laying State");
        curr_state =  S_LAYING;
      }
      ledYellow(false);
      break;
    case S_LAYING:
      // Detect if angle is inside of bounds
      if (abs(angle.read()) < DEG2RAD(45)) {
        plotter->info("Balacing State");
        curr_state =  S_BALANCING;
      }
      break;
    default:
      ledYellow(false);
      break;
  }

    // Sampling period is sampling_period
  byte current_time = millis();
  if ((byte)(current_time - last_sampled_time) >= sampling_period) {

    // Sampling period
    if ((byte)(current_time - last_sampled_time) > 21) {
      buzzer.playFrequency(440, 200, 15);
    }

    last_sampled_time = current_time;
    simulate_circuit();
  }
}

/**
 * Simluate the entire circuit
 */
void SegwayPID::simulate_circuit() {
  // IMU simulation doesn't depend on angle
  segway->simulate();

  if (curr_state == S_INITIALIZING) {
    init_cycle_count++;
    return;
  }

  if (curr_state == S_LAYING) {
    speed.write(0);
    segway->motor();
    return;
  }

  angle_corrector->simulate();
  pid_c->simulate();
  segway->motor();

  // plotter->plot(2, RAD2DEG(angle.read()));
  plotter->plot(0, speed.read());

}


/**
 * Build the circuit connecting all components together
 */
void SegwayPID::build_circuit() {
  // Connect Plant's inputs
  segway->angle = combined_angle;
  segway->angular_speed = angular_speed;
  segway->combined_angle = angle;

  // Connect the error calculator
  angle_corrector->in_0 = angle;
  angle_corrector->in_1 = center_angle;
  angle_corrector->out = corrected_angle;

  // PID controller
  pid_c->in = corrected_angle;
  pid_c->out = speed;

  // Connect Plant's output
  segway->speed = speed;
}

#endif
