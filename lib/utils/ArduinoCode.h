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

#ifndef ARDUINOCODE_H_
#define ARDUINOCODE_H_

#ifdef ARDUINO
#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#endif

#include <Plotter.h>


class ArduinoCode {
public:
  /** Serial Plotter */
  Plotter * plotter;
  /** Last sampled time */
  byte last_sampled_time = 0;

  ArduinoCode() {
    Wire.begin();
    delay(500);
    Serial.begin(115200);
    plotter = new Plotter(&Serial);
    delay(1000);
  }

  /** Arduino's loop function */
  virtual void loop() {
    // Nothing to do
  };

  /** Arduino's setup function*/
  virtual void setup() {
    // Nothing to do
  }

private:


};

#endif
