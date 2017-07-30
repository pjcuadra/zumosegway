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

#ifndef PID_H_
#define PID_H_

#include<Component.h>
#include<Gain.h>
#include<Adder.h>
#include<Integral.h>
#include<Limit.h>
#include<Derivative.h>

/**
 * PID Component
 */
class PID: public Component {
public:
  /**
   * Simulate the circuit component
   *
   * @param gain_p gain for the proportinal component
   * @param gain_i gain for the integral component
   * @param gain_d gain for the differential component
   */
  PID(float gain_p,
    float gain_i,
    float gain_d,
    float freq) {

    P = gain_p;
    I = gain_i;
    D = gain_d;

    a = (P + I/(2 * freq) + D * freq);
    b = (-P + I/(2 * freq) - 2 * freq * D);
    c = D * freq;

    u_1 = 0;
    e_1 = 0;
    e_2 = 0;
  }

  /**
   * Simulate the circuit component
   */
  inline float simulate(float in) {

    tmp = u_1 + a * in + b * e_1 + c * e_2;

    // Shift values
    e_2 = e_1;
    e_1 = in;
    u_1 = tmp;

    return tmp;
  }

private:
  /** Gain component for proportinal behaviour */
  float P;
  /** Gain component for integral behaviour */
  float I;
  /** Gain component for differential behaviour */
  float D;
  /** Lower saturation limit */
  int lower_limit;
  /** Upper saturation limit */
  int upper_limit;
  /** Simplification constant */
  float a;
  /** Simplification constant */
  float b;
  /** Simplification constant */
  float c;
  /** Previous output u(k-1) */
  float u_1;
  /** Previous input e(k-1) */
  float e_1;
  /** Previous input e(k-2) */
  float e_2;

  float tmp;
};

#endif
