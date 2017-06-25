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
class Integral: public Component {
public:
  /** Input port */
  Port in;
  /** Output port */
  Port out;

  /**
   * Constructor
   * @param lower lower saturation limit
   * @param upper upper saturation limit
   */
  Integral(float lower, float upper) : lim(lower, upper){
    lim.in = stored;
    lim.out = s_out;
  }

  /**
   * Simulate the circuit component
   */
  inline float simulate() {
    this->stored += in.read();
    lim.simulate();
    this->stored = s_out.read();
    return out.write(s_out.read());
  }

private:
  /** Internal store signal */
  Signal stored;
  /** Output limit signal */
  Signal s_out;
  /** Limit circuit component */
  Limit lim;
};

#endif
