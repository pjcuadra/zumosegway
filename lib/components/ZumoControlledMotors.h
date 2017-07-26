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
  /** Speed */
  Port speed;
  /** Debug output speed */
  Signal speed_out;

  /**
   *  Constructors
   */
  ZumoControlledMotors(double freq) {
    encoders.getCountsAndResetLeft();
    encoders.getCountsAndResetRight();

    this->freq = freq;

    left_pid = new PID(P, I, D, freq, -400, 400);
    right_pid = new PID(P, I, D, freq, -400, 400);

    left_pid->in = left_speed_e;
    left_pid->out = left_pid_out;
    right_pid->in = right_speed_e;
    right_pid->out = right_pid_out;

  }

  inline get_encoder_speed() {
    left_speed = encoders.getCountsAndResetLeft() * 2 * _PI * freq * count2cycle;
    right_speed = encoders.getCountsAndResetRight() * 2 * _PI * freq * count2cycle;
  }

  /**
   *  Simulate the component
   */
  inline double simulate() {
    get_encoder_speed();

    left_speed_e = speed.read() - left_speed;
    right_speed_e = speed.read() - right_speed;

    left_pid->simulate();
    right_pid->simulate();

    speed_out.write(left_speed);

    motors.setLeftSpeed((int16_t) left_pid_out.read());
    motors.setRightSpeed((int16_t) right_pid_out.read());

  }

private:
  /** Zumo motors */
  Zumo32U4Motors motors;
  /** Zumo Encoders */
  Zumo32U4Encoders encoders;
  /** Left Error Speed */
  Signal left_speed_e;
  /** Right Error Speed */
  Signal right_speed_e;
  /** Left PID controller */
  Signal left_pid_out;
  /** Right PID controller */
  Signal right_pid_out;
  /** Left speed */
  double left_speed;
  /** Right speed */
  double right_speed;
  /** Sampling Frequency */
  double freq;
  /** P parameterer of controller */
  const double P = 10 / (2 * _PI);
  /** I parameterer of controller */
  const double I = (freq * freq) / (2 * _PI);
  /** D parameterer of controller */
  const double D = 1 / (2 * _PI * (freq * freq));
  /** Zumo 100:1 motor gear ratio */
  const double gear_ratio = 100.37;
  /** Encoder count to cycle convertion constant */
  const double count2cycle = 1 / (12* gear_ratio);
  /** Left PID controller */
  PID * left_pid;
  /** Right PID controller */
  PID * right_pid;
};

#endif
