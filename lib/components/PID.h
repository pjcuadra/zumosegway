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

  /** Input Signal */
  Signal s_in;

  /** Integrator output signal  */
  Signal s_i_out;
  /** Derivator output signal  */
  Signal s_d_out;

  /** Proportinal gain output signal  */
  Signal s_g_p_out;
  /** Integral gain output signal  */
  Signal s_g_i_out;
  /** Derivate gain output signal  */
  Signal s_g_d_out;

  /** Intermediate adder output signal  */
  Signal s_add_tmp;

  /** Output Signal */
  Signal s_out;

  /**
   * Simulate the circuit component
   * @param gain_p gain for the proportinal component
   * @param gain_i gain for the integral component
   * @param gain_d gain for the differential component
   */
  PID(float gain_p, float gain_i, float gain_d) :
    P(gain_p),
    I(gain_i),
    D(gain_d),
    integral(-100, 100) {
    build_circuit();
  }

  /**
   * Simulate the circuit component
   * @param gain_p gain for the proportinal component
   * @param gain_i gain for the integral component
   * @param gain_d gain for the differential component
   * @param integral_lower lower saturation limit for the integral
   * @param integral_upper upper saturation limit for the integral
   */
  PID(float gain_p, float gain_i, float gain_d, float integral_lower, float integral_upper) :
    P(gain_p),
    I(gain_i),
    D(gain_d),
    integral(integral_lower, integral_upper) {
    build_circuit();
  }

  /**
   * Simulate the circuit component
   */
  inline float simulate() {

    s_in = in.read();

    // Simulation must be done as the signal propagates

    // Integral
    integral.simulate();
    // Differential
    diff.simulate();

    // Gains
    P.simulate();
    D.simulate();
    I.simulate();

    // Adders
    ad_0.simulate();
    ad_1.simulate();

    // Return the updated value
    return out.write(s_out.read());
  }

private:
  /** Gain component for proportinal behaviour */
  Gain P;
  /** Gain component for integral behaviour */
  Gain I;
  /** Gain component for differential behaviour */
  Gain D;
  /** Adder component */
  Adder ad_0;
  /** Adder component */
  Adder ad_1;
  /** Intergral component */
  Integral integral;
  /** Differential component */
  Derivative diff;

  /**
   * Interconnect all subblocks/subcomponents
   */
  void build_circuit() {
    // Connect to the integral and derivate
    integral.in = s_in;
    integral.out = s_i_out;

    diff.in = s_in;
    diff.out = s_d_out;

    // Connect the gain of every component
    P.in = s_in;
    P.out = s_g_p_out;

    I.in = s_i_out;
    I.out = s_g_i_out;

    D.in = s_d_out;
    D.out = s_g_d_out;

    // Add all together
    ad_0.in_0 = s_g_p_out;
    ad_0.in_1 = s_g_i_out;
    ad_0.out = s_add_tmp;

    ad_1.in_0 = s_add_tmp;
    ad_1.in_1 = s_g_d_out;
    ad_1.out = s_out;
  }

};

#endif
