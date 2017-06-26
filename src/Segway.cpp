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

// #define FILTERS 1

#include <PID.h>
#include <ZumoIMUFilters.h>
#include <ZumoIMU.h>
#include <ZumoMotors.h>
#include <SoftwareSerial.h>

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

// Controller buttoms
#define B_LEFT    48
#define B_UP      49
#define B_RIGHT   50
#define B_DOWN    51
#define B_SELECT  52
#define B_START   53
#define B_SQUARE  54
#define B_TRIAGLE 55
#define B_CROSS   56
#define B_CIRCLE  57

// States
#define S_STARTING           0
#define S_BALANCING          1
#define S_CALIBRATION        2
#define S_MOVING_FORWARD     3
#define S_MOVING_BACKWARDS   4
#define S_MOVING_FORWARD_F   5
#define S_MOVING_BACKWARDS_F 6

float calibrated_target_angle = -1.4;
const float move_angle = 2;
int curr_state = S_STARTING;
const int stablilizing = 100;
char c = ' ';

#define MEM_FILTER 2

float lastAngle[MEM_FILTER];
int counter;

/**
 * Setup function
 */
void setup() {

  Wire.begin();

  delay(500);

  Serial1.begin(115200);
  delay(1000);

  targetAngle = calibrated_target_angle;

  // Create zumo components

  #ifdef FILTERS
  imu = new ZumoIMUFilters();
  #else
  imu = new ZumoIMU();
  #endif
  motors = new ZumoMotors();

  // Compensate the deviation seen during experiments
  motors->dead_zone = 0;
  motors->right_scale = 0.80;

  // Create all components. Values taken from Zumo balancing example
  pid_controller = new PID(35, 15, 30, -40, 40);
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

  Serial1.println("INIT!");

  for (int i = 0; i < MEM_FILTER; i++) {
    lastAngle[i] = 0;
  }

  counter = 0;

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
    case S_STARTING:
      if (counter > stablilizing) {
        curr_state = S_BALANCING;
        speed = 0;
      }

      Serial1.println("Starting");
      break;
    case S_BALANCING:
      targetAngle = calibrated_target_angle;
      ledYellow(false);

      switch (c) {
        case B_SELECT:
          curr_state = S_CALIBRATION;
          break;
        case B_UP:
          curr_state = S_MOVING_FORWARD;
          targetAngle = calibrated_target_angle - move_angle;
          break;
        case B_DOWN:
          curr_state = S_MOVING_BACKWARDS;
          targetAngle = calibrated_target_angle + move_angle;
          break;
        case B_TRIAGLE:
          curr_state = S_BALANCING;
          break;
        case B_CROSS:
          curr_state = S_BALANCING;
          break;
        default:
          break;
      }


      break;
    case S_CALIBRATION:
      ledYellow(true);
      switch (c) {
        case B_SELECT:
          curr_state = S_BALANCING;
          break;
        case B_UP:
          calibrated_target_angle -= 0.1;
          targetAngle = calibrated_target_angle;
          break;
        case B_DOWN:
          calibrated_target_angle += 0.1;
          targetAngle = calibrated_target_angle;
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
        targetAngle = calibrated_target_angle;
        curr_state = S_BALANCING;
      }
    case S_MOVING_BACKWARDS:
      if (c == B_UP) {
        targetAngle = calibrated_target_angle;
        curr_state = S_BALANCING;
      }
      if (c == B_START) {
        targetAngle = calibrated_target_angle;
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
    Serial1.print("Time: ");
    Serial1.print(current_time);
    Serial1.println(" ms");

    Serial1.print("Sampling Time: ");
    Serial1.print((byte)(current_time - sampling_time));
    Serial1.println(" ms");

    Serial1.print("Target Angle: ");
    Serial1.println(targetAngle.read());

    sampling_time = current_time;
    simulate_circuit();
  }
}

/**
 * Simluate the entire circuit
 */
void simulate_circuit() {
  float tmpAngle = 0;

  // IMU simulation doesn't depend on angle
  imu->simulate();

  // Shift all right
  // for (int i = MEM_FILTER - 1; i >= 1; i--) {
  //   tmpAngle += lastAngle[i - 1];
  //   lastAngle[i] = lastAngle[i - 1];
  // }
  // float filter = 0.98;
  // angle = filter * angle.read() + (1 - filter) * lastAngle[0] ;

  // lastAngle[0] = angle.read();

  Serial1.print("Angle: ");
  Serial1.println(angle.read());


  if (curr_state == S_STARTING) {
    counter++;
    return;
  }

  // Have all simulation inside if to prevent  accumulating error when
  // angle > 45
  if (abs(angle.read()) > 45) {
    speed = 0;
    Serial1.println("Angle greater than 45");
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
