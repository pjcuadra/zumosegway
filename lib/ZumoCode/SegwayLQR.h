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

  float u = 0;

  static const int outputs = 3;
  static const int states = 6;

  const float K[states] = {0, -93.002, 2277.1, 172.66, -23.831, 0.49942};

  // Observer Model
  const float A[states][states] = {{0, 1, 0, 0, 0, 0},
                                    {0, -0.4132, 0, 0, 0.007851, 0.019},
                                    {0, 0, 0, 1, 0, 0},
                                    {0, -7.327, 173.9, 0, 0.1392, 0.3369},
                                    {0, 0, 0, 0, 0, 1},
                                    {0, 0, 0, 0, -676, -52}};

  const float B[states] = {0, 0, 0, 0, 0, 52.9};

  const float C[outputs][states] = {{0, 0, 1, 0, 0, 0},
                                    {0, 0, 0, 1, 0, 0},
                                    {0, 0, 0, 0, 1, 0}};

  const float L[states][outputs] = {{0, 0, 0},
                                    {-8.1111, -0.30223,  -0.88620},
                                    {-0.40517, 1.0007, -0.00024511},
                                    {1.0007, -5.3065, -15.730},
                                    {-0.00024511, -15.730, -46.702},
                                    {0.0064736, 613.57, 1821.4}};


  float a_angle;
  float lp_a_angle;
  float g_read;
  float g_angular_speed;
  float hp_g_angular_speed;
  float c_angle;
  float e_omega;

  void loop();
  void setup();
  void setMotors();
  void get_angle();
  void get_angular_speed();
  void get_omega();

  void filter_angle();
  void filter_angular_speed();

  void get_combined_angle();
  void compensator();

private:



};

#endif
