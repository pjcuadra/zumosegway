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

#ifndef FILTER_H_
#define FILTER_H_

#include<Component.h>
#include<Gain.h>
#include<Adder.h>
#include<Integral.h>
#include<Limit.h>
#include<Derivative.h>

/**
 * State Variable Component
 */
template<int degree>
class Filter: public Component {
public:
  /**
   * Simulate the circuit component
   *
   *          Y(z)     a[0] + a[1]z^-1 + ... + a[n]z^(n)
   *  H(z) = ------ = -----------------------------------
   *          X(z)     b[0] + b[1]z^-1 + ... + b[n]z^(n)
   *
   * @param a a coefficients
   * @param b b coefficients
   */
  Filter(
    const float b[degree + 1],
    const float a[degree + 1]) {

    // Copy the coefficients
    for (int i = 0; i < degree + 1; i++) {
      this->a[i] = a[i];
      this->b[i] = b[i];
    }

    for (int i = 0; i < degree; i++) {
      // Init states
      x[i] = 0;
      y[i] = 0;
    }
  }

  /**
   * Simulate the circuit component
   */
  inline float filter(float in) {

    float tmp_out = in * b[0];

    // Update the state
    for (int i = 0; i < degree; i++){
      tmp_out += b[i + 1] * x[i] - a[i + 1] * y[i];
    }

    // Shift states
    for (int i = degree - 1; i > 0; i--){
      x[i] = x[i - 1];
      y[i] = y[i - 1];
    }

    x[0] = in;
    y[0] = tmp_out/a[0];

    return y[0];
  }

private:
  /** B coeficients */
  float b[degree + 1];
  /** A coeficients */
  float a[degree + 1];
  /** Previous inputs stored states */
  float x[degree];
  /** Previous outputs stored states */
  float y[degree];

};

#endif
