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

#ifndef GAIN_H_
#define GAIN_H_

#include<Component.h>

/**
 * Gain Component
 */
class Gain: public Component {
public:
  /** Input port */
  Port in;
  /** Output port */
  Port out;

  /**
   * Constructor
   * @param gain_k gain
   */
  Gain(float gain_k) {
    this->gain_k = gain_k;
  }

  /**
   * Simulate the circuit component
   */
  inline float simulate() {
    return out.write(gain_k * in.read());
  }

private:
  /** Gain */
  float gain_k;

};

#endif
