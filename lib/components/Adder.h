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
#ifndef ADDER_H_
#define ADDER_H_

#include<Component.h>

/**
 * Adder Component
 */
class Adder: public Component {
public:
  /** Input port 0 */
  Port in_0;
  /** Input port 1 */
  Port in_1;
  /** Output port */
  Port out;

  /**
   * Simulate the circuit component
   */
  inline double simulate() {
    return out.write(in_0.read() + in_1.read());
  }
};

#endif
