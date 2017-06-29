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
   * Add a value to the plot
   * @param plot_num number of the plot
   * @param value value to be set
   */
  inline static void plot(int plot_num, float value) {
    Serial1.print(plot_num + 1);
    Serial1.print(" ");
    Serial1.println(value);
  }

  /**
   * Send information
   * @param message info message
   */
  inline static void info(char message[30]) {
    Serial1.print(0);
    Serial1.print(" [INFO] ");
    Serial1.println(message);
  }

  /**
   * Send information
   * @param message message to be displayed
   * @param value value to be concatenated with the message
   */
  inline static void info(char message[30], float value) {
    Serial1.print(0);
    Serial1.print(" [INFO] ");
    Serial1.print(message);
    Serial1.print(" ");
    Serial1.println(value);
  }

  /**
   * Configure plot
   * @param plot_num number of the plot
   * @param cmd configure command
   */
  inline static void config_plot(int plot_num, char cmd[30]) {
    Serial1.print(0);
    Serial1.print(" [CONFIG] plot:{");
    Serial1.print(plot_num);
    Serial1.print("} ");
    Serial1.println(message);
  }

  /**
   * Initialize the plotter
   */
  inline static void init() {
    Serial1.begin(baudrate);
  }

private:
  /** Defualt baudrate */
  static const int baudrate = 115200;
};

#endif
