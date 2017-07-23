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

#ifndef PLOTTER_H_
#define PLOTTER_H_

#ifdef ARDUINO
#include <Arduino.h>
#endif

/**
 * Plotter
 */
class Plotter {
public:

  /**
   * Constructor
   */
  Plotter(Serial_ * serial) {
    this->serial = serial;
  }

  /**
   * Add a value to the plot
   * @param plot_num number of the plot
   * @param value value to be set
   */
  inline void plot(int plot_num, double value) {
    serial->print(plot_num + 1);
    serial->print(" ");
    serial->println(value);
  }

  /**
   * Send information
   * @param message info message
   */
  inline void info(const char * message) {
    serial->print(0);
    serial->print(" [INFO] ");
    serial->println(message);
  }

  /**
   * Send information
   * @param message message to be displayed
   * @param value value to be concatenated with the message
   */
  inline void info(const char * message, double value) {
    serial->print(0);
    serial->print(" [INFO] ");
    serial->print(message);
    serial->print(" ");
    serial->println(value);
  }

  /**
   * Configure plot
   * @param plot_num number of the plot
   * @param cmd configure command
   */
  inline void config_plot(int plot_num, const char * cmd) {
    serial->print(0);
    serial->print(" [CONFIG] plot:{");
    serial->print(plot_num);
    serial->print("} ");
    serial->println(cmd);
  }

private:
  /** Serial Device */
  Serial_ * serial;
};

#endif
