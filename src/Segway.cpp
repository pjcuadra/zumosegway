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

#include <PID.h>
#include <ZumoIMUFilters.h>
#include <ZumoIMU.h>
#include <ZumoMotors.h>
#include <SoftwareSerial.h>

#define FILTERS 1

// Needed for starting balancing
Zumo32U4ButtonA buttonA;

// Components
PID * pid_controller;
Adder * error_adder;
Gain * inv_feedback;
Limit * lim_output;
#ifdef FILTERS
ZumoIMUFilters * imu;
#else
ZumoIMU * imu;
#endif
ZumoMotors * motors;

// Signals
Signal target_angle;
Signal pid_out;
Signal speed;
Signal feedback;
Signal angle;
Signal angle_error;

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

/** Add/substracted value to move forward/backward */
const float move_angle = 1;
/** Initializing Cycles */
const int init_cycles = 100;
/** Oscilation period (Experimentally obtained) */
const float t_cr = 0.150;
/** Proportional gain till oscilation */
const float k_cr = 55;
/** Proportional gain */
const float P = 0.6 * k_cr;
/** Integral gain */
const float I = 1 / (0.5 * t_cr);
/** Derivative gain */
const float D = 1 / (0.125 * t_cr);

/** Serial received character */
char c = ' ';
/** Calibrated Target Angle */
#ifndef FILTERS
float calibrated_target_angle = -2;
#else
float calibrated_target_angle = -1.4;
#endif
/** Current state */
zumo_states_e curr_state = S_INITIALIZING;
/** Initializing cycles init_cycle_count */
int init_cycle_count;

/**
 * Setup function
 */
void setup() {

  Wire.begin();
  delay(500);
  Serial1.begin(115200);
  delay(1000);

  target_angle = calibrated_target_angle;

  // Create zumo components
  #ifdef FILTERS
  imu = new ZumoIMUFilters();
  #else
  imu = new ZumoIMU();
  #endif
  motors = new ZumoMotors();

  // Compensate the deviation seen during experiments
  motors->dead_zone = 0;

  // Send PID values
  Plotter::info("P", P);
  Plotter::info("I", I);
  Plotter::info("D", D);

  // Configure the plots
  Plotter::config_plot(0, "title:{Current Time}");
  Plotter::config_plot(0, "period:{20}");

  Plotter::config_plot(1, "title:{Sampling Period}");
  Plotter::config_plot(0, "period:{20}");

  Plotter::config_plot(2, "title:{Target Value}");
  Plotter::config_plot(0, "period:{20}");

  Plotter::config_plot(3, "title:{Angle}");
  Plotter::config_plot(0, "period:{20}");

  // Create all components. Values taken from Zumo balancing example
  pid_controller = new PID(P, I, D, -40, 40);
  error_adder = new Adder();
  inv_feedback = new Gain(-1);
  lim_output = new Limit(-400, 400);

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

  Plotter::info("Initializing State");
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
        Plotter::info("Balacing State");
        speed = 0;
      }

      break;
    case S_BALANCING:
      // Update the target angle
      target_angle = calibrated_target_angle;
      ledYellow(false);

      switch (c) {
        case B_SELECT: // Start calibrating state
          curr_state = S_CALIBRATING;
          Plotter::info("Calibration State");
          break;
        case B_UP: // Start moving foward
          curr_state = S_MOVING_FORWARD;
          Plotter::info("Moving Forward State");
          target_angle = calibrated_target_angle - move_angle;
          break;
        case B_DOWN: // Start moving backwards
          curr_state = S_MOVING_BACKWARDS;
          Plotter::info("Moving Backwards State");
          target_angle = calibrated_target_angle + move_angle;
          break;
        case B_TRIAGLE:
          curr_state = S_BALANCING;
          Plotter::info("Balancing State");
          break;
        case B_CROSS:
          curr_state = S_BALANCING;
          Plotter::info("Balancing State");
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
        case B_UP:
          calibrated_target_angle -= 0.1;
          target_angle = calibrated_target_angle;
          break;
        case B_DOWN:
          calibrated_target_angle += 0.1;
          target_angle = calibrated_target_angle;
          break;
        case B_LEFT:
          if (motors->right_scale == 1) {
            motors->left_scale -= 0.01;
          } else {
            motors->right_scale += 0.01;
          }

          break;
        case B_RIGHT:
          if (motors->left_scale == 1) {
            motors->right_scale -= 0.01;
          } else {
            motors->left_scale += 0.01;
          }
          break;
        default:
          break;
      }
      break;
    case S_MOVING_FORWARD:
      if (c == B_DOWN) {
        target_angle = calibrated_target_angle;
        curr_state = S_BALANCING;
      }
    case S_MOVING_BACKWARDS:
      if (c == B_UP) {
        target_angle = calibrated_target_angle;
        curr_state = S_BALANCING;
      }
      if (c == B_START) {
        target_angle = calibrated_target_angle;
        curr_state = S_BALANCING;
      }
      ledYellow(false);
      break;
    default:
      ledYellow(false);
      break;
  }

  #ifndef FILTERS
    imu->updateAngleGyro();
  #endif

    // Sampling period is sampling_period
  byte current_time = millis();
  if ((byte)(current_time - sampling_time) >= sampling_period) {
    // Current Time
    Plotter::plot(0, current_time);

    // Sampling period
    Plotter::plot(1, (byte)(current_time - sampling_time));

    // Target Angle
    Plotter::plot(2, target_angle.read());

    Serial.println();

    sampling_time = current_time;
    simulate_circuit();
  }
}

/**
 * Simluate the entire circuit
 */
void simulate_circuit() {
  // IMU simulation doesn't depend on angle
  imu->simulate();

  Plotter::plot(3, angle.read());
  Serial.println(angle.read());

  if (curr_state == S_INITIALIZING) {
    init_cycle_count++;
    return;
  }

  // Have all simulation inside if to prevent  accumulating error when
  // angle > 45
  if (abs(angle.read()) > 45) {
    speed = 0;
    Plotter::info("Angle greater than 45");
  } else {
    inv_feedback->simulate();
    error_adder->simulate();
    pid_controller->simulate();
    lim_output->simulate();
  }

  motors->simulate();
}

/**
 * Build the circuit connecting all components together
 */
void build_circuit() {
  imu->out = angle;

  // Change sign of the angle
  inv_feedback->in = angle;
  inv_feedback->out = feedback;

  // Error calculator
  error_adder->in_0 = target_angle;
  error_adder->in_1 = feedback;
  error_adder->out = angle_error;

  // PID controller
  pid_controller->in = angle_error;
  pid_controller->out = pid_out;

  // Limit the PID output
  lim_output->in = pid_out;
  lim_output->out = speed;

  // Set motor speed
  motors->left_speed = speed;
  motors->right_speed = speed;
}

#endif
