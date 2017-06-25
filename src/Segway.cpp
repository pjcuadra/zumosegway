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
#include <ZumoIMU.h>
#include <ZumoMotors.h>

// Needed for starting balancing
Zumo32U4ButtonA buttonA;

// Components
PID * pid_controller;
Adder * error_adder;
Gain * inv_feedback;
Limit * lim_output;
ZumoIMU * imu;
ZumoMotors * motors;

// Signals
Signal targetAngle;
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

/**
 * Setup function
 */
void setup() {

  Wire.begin();

  delay(500);

  targetAngle = -1.9;

  // Create zumo components
  imu = new ZumoIMU();
  motors = new ZumoMotors();

  // Compensate the deviation seen during experiments
  motors->right_scale = 0.97;
  motors->dead_zone = 50;

  // Create all components. Values taken from Zumo balacning example
  pid_controller = new PID(35, 5, 10, -40, 40);
  error_adder = new Adder();
  inv_feedback = new Gain(-1);
  lim_output = new Limit(-400, 400);

  // Build the circuit connecting all components together
  build_circuit();

  // Display the angle until the user presses A.
  while (!buttonA.getSingleDebouncedRelease()) {
    // Just wait ;)
  }

}

/**
 * Loop function
 */
void loop() {
  // Update the angle using the gyro as often as possible.
  imu->updateAngleGyro();

  // Sampling period is sampling_period
  byte current_time = millis();
  if ((byte)(current_time - sampling_time) >= sampling_period) {
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

  // Have all simulation inside if to prevent accumulating error when
  // angle > 45
  if (abs(angle.read()) > 45) {
    speed = 0;
  } else {
    inv_feedback->simulate();
    error_adder->simulate();
    pid_controller->simulate();
    lim_output->simulate();
  }

  // This has to be outside to stop the motors
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
  error_adder->in_0 = targetAngle;
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
