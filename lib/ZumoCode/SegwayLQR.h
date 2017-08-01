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

#ifndef SEGWAYLQR_H_
#define SEGWAYLQR_H_

// Misc.
#include <ArduinoCode.h>

// Debugging utils
#include <Plotter.h>

// Components
#include <ZumoSegway.h>
#include <ZumoSegwayIMU.h>
#include <PID.h>
#include <Integral.h>

class SegwayLQR: public ArduinoCode {
public:
  Zumo32U4ButtonA buttonA;
  Zumo32U4Motors motors;
  /** Zumo Encoders */
  Zumo32U4Encoders encoders;

  L3G gyro;
  LSM303 compass;

  /** Zumo 100:1 motor gear ratio */
  const float gear_ratio = 100.37;
  /** Encoder count to cycle convertion constant */
  const float count2cycle = 1 / (12* gear_ratio);

  // This is the average reading obtained from the gyro's Y axis
  // during calibration.
  float gyroOffsetY;

  // This variable holds our estimation of the robot's angle based
  // on the gyro and the accelerometer.  A value of 0 means the
  // robot is perfectly vertical.  A value of -90 means that the
  // robot is horizontal and the battery holder is facing down.  A
  // value of 90 means that the robot is horizontal and the battery
  // holder is facing up.
  float angle = 0;

  // This is just like "angle", but it is based solely on the
  // accelerometer.
  float aAngle = 0;

  float freq = 50;

  static const int outputs = 2;
  static const int states = 4;

  // const float K[states] = {3.1623, 478.3485, 9.0426, 26.1122};
  const float K[states] = {80, 4369.3049, 18.1538, 69.6881};
  const float scale = 0.5;
  const float target_angle = -0.5;
  float x[states] = {0, 0, 0, 0};


  float a_angular_position = 0;
  float g_angular_speed = 0;
  float c_angle = 0;
  float e_angular_speed = 0;
  float e_angular_position = 0;

  void loop();
  void setup();
  void setMotors();
  float get_zumo_angular_position();
  float get_zumo_angular_speed();
  float get_motor_angular_position();
  float get_motor_angular_speed();

private:



};

#endif
