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

#ifndef SEGWAYPID_H_
#define SEGWAYPID_H_

// Misc.
#include <ArduinoCode.h>

// Debugging utils
#include <Plotter.h>

// Components
#include <ZumoSegway.h>
#include <ZumoSegwayIMU.h>
#include <PID.h>
#include <Integral.h>

class SegwayPID: public ArduinoCode {
public:
  Zumo32U4ButtonA buttonA;
  Zumo32U4Motors motors;

  L3G gyro;
  LSM303 compass;

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

  float P = 40;
  float I = 576;
  float D = 0.5625;

  float a = (P + I/(2 * freq) + D * freq);
  float b = (-P + I/(2 * freq) - 2 * freq * D);
  float c = D * freq;
  float a_angle;
  float lp_a_angle;
  float g_read;
  float g_angular_speed;
  float hp_g_angular_speed;
  float c_angle;

  void loop();
  void setup();
  void setMotors();
  void correctAngleAccel();
  void updateAngleGyro();
  void get_angle();
  void get_angular_speed();

  void filter_angle();
  void filter_angular_speed();

  void get_combined_angle();

private:



};

#endif
