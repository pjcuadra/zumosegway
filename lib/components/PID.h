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

class PID: public Component<1, 1> {
public:
  Gain P;
  Gain I;
  Gain D;
  Adder ad_0;
  Adder ad_1;
  Integral integral;
  Derivative diff;

  PID(float gain_p, float gain_i, float gain_d) :
    P(gain_p),
    I(gain_i),
    D(gain_d),
    integral(-100, 100) {
    build_circuit();
  }

  PID(float gain_p, float gain_i, float gain_d, float integral_lower, float integral_upper) :
    P(gain_p),
    I(gain_i),
    D(gain_d),
    integral(integral_lower, integral_upper) {
    build_circuit();
  }

  inline void connect_input(int index, float *signal){
    integral.connect_input(index, signal);
    diff.connect_input(index, signal);
    P.connect_input(index, signal);
    Component<1, 1>::connect_input(index, signal);
  }

  inline void connect_input(float *signal){
    connect_input(0, signal);
  }

  inline void connect_output(int index, float *signal){
    ad_1.connect_output(index, signal);
    Component<1, 1>::connect_output(index, signal);

  }

  inline void connect_output(float *signal){
    connect_output(0, signal);
  }


  inline float simulate() {

    integral.simulate();
    diff.simulate();

    P.simulate();
    I.simulate();
    D.simulate();

    ad_0.simulate();
    ad_1.simulate();

    set_probe(integral.get_output());

    return ad_1.get_output();
  }

private:
  void build_circuit() {
    // Connect to the integral and derivate
    integral.connect_input(get_input_signal());
    diff.connect_input(get_input_signal());

    // Connect the gain of every component
    P.connect_input(get_input_signal());
    I.connect_input(integral.get_output_signal());
    D.connect_input(diff.get_output_signal());

    // Connect both adders together
    ad_1.connect_input(ad_0.get_output_signal());

    // Add all together
    ad_0.connect_input(0, P.get_output_signal());
    ad_0.connect_input(1, I.get_output_signal());
    ad_1.connect_input(1, D.get_output_signal());

    ad_1.connect_output(get_output_signal());
  }

};

#endif
