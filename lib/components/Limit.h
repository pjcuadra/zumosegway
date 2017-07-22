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
#include<Port.h>

/**
 * Limit Component
 */
class Limit: public Component {
public:
  /** Input port */
  Port in;
  /** Output port */
  Port out;

  /**
   * Constructor
   * @param lower lower limit
   * @param upper upper limit
   */
  Limit(double lower, double upper) {
    this->upper = upper;
    this->lower = lower;
  }

  /**
   * Simulate the circuit component
   */
  inline double simulate() {
    #ifdef ARDUINO
      return out.write(constrain(in.read(), lower, upper));
    #else
      if (in.read() < lower) {
        return out.write(lower);
      }

      if (in.read() > upper) {
        return out.write(upper);
      }
    #endif
  }

private:
  /** Upper limit */
  double upper;
  /** Lower limit */
  double lower;
};

#endif
