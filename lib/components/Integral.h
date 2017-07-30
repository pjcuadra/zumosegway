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

#ifndef INTEGRAL_H_
#define INTEGRAL_H_

#ifdef ARDUINO
#include <Arduino.h>
#include <Zumo32U4.h>
#endif

#include<Component.h>
#include<Limit.h>

/**
 * Integral Component
 */
class Integral: public Component {
public:
  /**
   * Constructor
   * @param lower lower saturation limit
   * @param upper upper saturation limit
   * @param freq sampling frequency
   */
  Integral(float lower, float upper, float freq) {
    this->lower = lower;
    this->upper = upper;
    this->freq = freq;

    x_1 = 0;
    y_1 = 0;
  }

  /**
   * Simulate the circuit component
   */
  inline float calculate(float in) {
    float tmp = y_1 + (in + x_1) / (2 *  freq);

    /*
     * Discrete integrator
     * y(k) = y(k - 1) + (x(k) + x(k - 1)) * Ts / 2
     */

    // Update stored states
    y_1 = constrain(tmp, lower, upper);
    x_1 = in;

    return y_1;
  }

private:
  /** Limit circuit component */
  float lower;
  float upper;
  /** Sampling frequency */
  float freq;

  float x_1;
  float y_1;
};

#endif
