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

#ifndef PID_H_
#define PID_H_

#include<Component.h>
#include<Gain.h>
#include<Adder.h>
#include<Integral.h>
#include<Limit.h>
#include<Derivative.h>

/**
 * PID Component
 */
class PID: public Component {
public:
  /** Input port */
  Port in;
  /** Output port */
  Port out;

  /**
   * Simulate the circuit component
   *
   * @param gain_p gain for the proportinal component
   * @param gain_i gain for the integral component
   * @param gain_d gain for the differential component
   * @param freq sampling frequency
   * @param lower_limit lower saturation limit
   * @param upper_limit upper saturation limit
   */
  PID(double gain_p,
    double gain_i,
    double gain_d,
    double freq,
    int lower_limit,
    int upper_limit) {

    P = gain_p;
    I = gain_i;
    D = gain_d;

    this->lower_limit = lower_limit;
    this->upper_limit = upper_limit;

    a = (P + I/((double)2 * freq) + D * freq);
    b = (-P + I/((double)2 * freq) - 2 * freq * D);
    c = D * freq;

    u_1 = 0;
    e_1 = 0;
    e_2 = 0;
  }

  /**
   * Simulate the circuit component
   *
   * @param gain_p gain for the proportinal component
   * @param gain_i gain for the integral component
   * @param gain_d gain for the differential component
   */
  PID(double gain_p,
    double gain_i,
    double gain_d,
    double freq) {

    P = gain_p;
    I = gain_i;
    D = gain_d;

    this->lower_limit = -10000;
    this->upper_limit = 10000;

    a = (P + I/(2 * freq) + D * freq);
    b = (-P + I/(2 * freq) - 2 * freq * D);
    c = D * freq;

    u_1 = 0;
    e_1 = 0;
    e_2 = 0;
  }

  /**
   * Simulate the circuit component
   */
  inline double simulate() {

    out.write(constrain(u_1 + a * in.read() + b * e_1 + c * e_2,
      this->lower_limit,
      this->upper_limit));

    // Shift values
    e_2 = e_1;
    e_1 = in.read();
    u_1 = out.read();

    return out.read();
  }

private:
  /** Gain component for proportinal behaviour */
  double P;
  /** Gain component for integral behaviour */
  double I;
  /** Gain component for differential behaviour */
  double D;
  /** Lower saturation limit */
  int lower_limit;
  /** Upper saturation limit */
  int upper_limit;
  /** Simplification constant */
  double a;
  /** Simplification constant */
  double b;
  /** Simplification constant */
  double c;
  /** Previous output u(k-1) */
  double u_1;
  /** Previous input e(k-1) */
  double e_1;
  /** Previous input e(k-2) */
  double e_2;
};

#endif
