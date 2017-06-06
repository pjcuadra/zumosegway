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

#include<Component.h>
#include<Limit.h>

/**
 * Integral Component
 */
class Integral: public Component<1, 1> {
public:

  /**
   * Constructor
   * @param lower lower saturation limit
   * @param upper upper saturation limit
   */
  Integral(float lower, float upper) : lim(lower, upper){
    lim.connect_input(&stored);
    lim.connect_output(&stored);
  }

  /**
   * Simulate the circuit component
   */
  inline float simulate() {
    this->stored += get_input();
    return write_output(lim.simulate());
  }

private:
  /** Internal store value */
  float stored = 0;
  /** Limit circuit component */
  Limit lim;
};

#endif
