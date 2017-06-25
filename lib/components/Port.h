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

#ifndef PORT_H_
#define PORT_H_

#include<Signal.h>

/**
 * Port Class
 */
class Port {
public:

  /**
   * Constructor
   */
  Port() {
    port_signal = NULL;
  }

  Port& operator= (Signal &signal_var) {
    this->port_signal = signal_var.get();
    return *this;
  }

  // void bind(Port * to_bind_port) {
  //   this = to_bind_port;
  // }

  /**
   * Read the value from the port
   */
  float read() {
    if (!check()) {
      return 0;
    }

    return *port_signal;
  }

  /**
   * Write a value to the port
   */
  float write(float value) {
    if (!check()) {
      return 0;
    }

    *port_signal = value;
    return value;
  }

  /**
   * Check if the port is connected
   */
  inline bool check() {
    return port_signal != NULL;
  }

private:
  /** Port signal */
  float * port_signal;

};

#endif
