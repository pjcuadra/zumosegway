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

#ifndef SIGNAL_H_
#define SIGNAL_H_

/**
 * Signal Class
 */
class Signal {
public:

  /**
   * Constructor
   */
  Signal() {
    signal_value = 0;
  }

  /**
   * Read the value from the signal
   */
  float read() {
    return signal_value;
  }

  /**
   * Write a value to the signal
   */
  float write(float value) {
    this->signal_value = value;
    return this->signal_value;
  }

  /**
   * Get the signal's reference
   */
  float * get() {
    return &this->signal_value;
  }

  /**
   * Equal operator overloading
   */
  Signal& operator= (Signal &other) {

    // check for self-assignment
     if(&other == this)
         return *this;

     this->signal_value = other.signal_value;

     return *this;

  }

  /**
   * Equal operator overloading
   */
  Signal& operator= (float value) {
    this->signal_value = value;
    return *this;
  }

  /**
   * += operator overloading
   */
  Signal& operator+= (Signal &other) {

     this->signal_value += other.signal_value;

     return *this;
  }

  /**
   * += operator overloading
   */
  Signal& operator+= (float other) {

     this->signal_value += other;

     return *this;
  }

private:
  /** Signal value */
  float signal_value;

};

#endif
