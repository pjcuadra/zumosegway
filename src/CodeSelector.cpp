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
#include <Commands.h>
#include <CodeSelector.h>

// Code Classes
#include <SegwayPID.h>
#include <SegwayLQR.h>
#include <MotorSpeedControl.h>
#include <IMU.h>

// Code Class pointer
ArduinoCode * code;
// Code Class selector
code_selector_e selector = SEGWAY_LQR;

void setup() {
  // Select the code class to run
  switch (selector) {
    case SEGWAY_PID:
      code = new SegwayPID();
      break;
    case SEGWAY_LQR:
      code = new SegwayLQR();
      break;
    case ZUMOMOTOR_SPEED_PID:
      code = new MotorSpeedControl();
      break;
    case IMU_PROCESSING_DEMO:
      code = new IMU();
      break;
    default:
      code = new SegwayPID();
  }

  code->setup();
}

void loop() {
  code->loop();
}

#endif
