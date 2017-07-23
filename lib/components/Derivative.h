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

#ifndef DERIVATIVE_H_
#define DERIVATIVE_H_

#include<Component.h>

/**
 * Derivative Component
 */
class Derivative: public Component {
public:
  /* Input port */
  Port in;
  /* Output port */
  Port out;

  /**
   * Constructor
   * @param freq sampling frequency
   */
  Derivative(double freq) {
    this->freq = freq;
  }

  /**
   * Simulate the circuit component
   */
  inline double simulate() {
    double input = in.read();
    double tmp = (input - stored)*freq;
    stored = input;
    return out.write(tmp);
  }

private:
  /** Internal store variable */
  double stored = 0;
  /** Sampling Time */
  double freq = 1;

};

#endif
