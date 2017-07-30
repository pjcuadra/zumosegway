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

#ifndef ZUMOCONTROLLEDMOTORS_H_
#define ZUMOCONTROLLEDMOTORS_H_

#ifdef ARDUINO
#include <Arduino.h>
#include <Zumo32U4.h>
#endif

#include<Component.h>
#include<Util.h>
#include<PID.h>

/**
 * ZumoControlledMotors Class
 */
class ZumoControlledMotors: public Component {
public:
  /**
   *  Constructors
   */
  ZumoControlledMotors(float freq) {
    encoders.getCountsAndResetLeft();
    encoders.getCountsAndResetRight();

    this->freq = freq;

    left_pid = new PID(P, I, D, freq);
    right_pid = new PID(P, I, D, freq);

  }

  inline void get_encoder_speed() {
    left_speed = encoders.getCountsAndResetLeft() * 2 * _PI * freq * count2cycle;
    right_speed = encoders.getCountsAndResetRight() * 2 * _PI * freq * count2cycle;
  }

  /**
   *  Simulate the component
   */
  inline float simulate(float speed) {
    float tmp = 0;
    get_encoder_speed();

    left_speed_e = speed - left_speed;
    right_speed_e = speed - right_speed;

    tmp = left_pid->simulate(left_speed_e);
    motors.setLeftSpeed((int16_t) tmp);

    tmp = right_pid->simulate(right_speed_e);
    motors.setRightSpeed((int16_t) tmp);

    return tmp;

  }

private:
  /** Zumo motors */
  Zumo32U4Motors motors;
  /** Zumo Encoders */
  Zumo32U4Encoders encoders;
  /** Left Error Speed */
  float left_speed_e;
  /** Right Error Speed */
  float right_speed_e;
  /** Left speed */
  float left_speed;
  /** Right speed */
  float right_speed;
  /** Sampling Frequency */
  float freq;
  /** P parameterer of controller */
  const float P = 20;
  /** I parameterer of controller */
  const float I = 200;
  /** D parameterer of controller */
  const float D = 0;
  /** Zumo 100:1 motor gear ratio */
  const float gear_ratio = 100.37;
  /** Encoder count to cycle convertion constant */
  const float count2cycle = 1 / (12* gear_ratio);
  /** Left PID controller */
  PID * left_pid;
  /** Right PID controller */
  PID * right_pid;
};

#endif
