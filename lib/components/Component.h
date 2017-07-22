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

#ifndef COMPONENT_H_
#define COMPONENT_H_

#include<Port.h>
#include<Signal.h>

/**
 * Component Abstract Class
 */
class Component {
public:

  /**
   * Set the value of an internal probe variable (for debug)
   * @param value value to be stored
   */
  inline void set_probe(double value) {
    probe_value = value;
  }

  /**
   * Get the value of the internal probe variable (for debug)
   * @return the value of the probe variable
   */
  inline float probe() {
    return probe_value;
  }

private:
  /** Probe Variable */
  float probe_value;

};

#endif
