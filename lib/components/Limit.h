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

#ifndef Limit_H_
#define Limit_H_

#ifdef ARDUINO
#include <Arduino.h> // Needed for the constrain function
#endif

#include<Component.h>

/**
 * Limit Component
 */
class Limit: public Component<1, 1> {
public:
  /**
   * Constructor
   * @param lower lower limit
   * @param upper upper limit
   */
  Limit(float lower, float upper) {
    this->upper = upper;
    this->lower = lower;
  }

  /**
   * Simulate the circuit component
   */
  inline float simulate() {
    #ifdef ARDUINO
      return write_output(constrain(get_input(), lower, upper));
    #else
      if (get_input() < lower) {
        return write_output(lower);
      }

      if (get_input() > upper) {
        return write_output(upper);
      }
    #endif
  }

private:
  /** Upper limit */
  float upper;
  /** Lower limit */
  float lower;
};

#endif
