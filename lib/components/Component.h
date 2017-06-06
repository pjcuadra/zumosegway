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

// Base class
template<int inputs, int outputs>
  class Component {
  public:

    Component() {
      for (int i = 0; i < outputs; i++) {
        out[i] = new float(0);
      }
    }

    inline void connect_input(int index, float *signal) {
      in[index] = signal;
    }

    inline void connect_input(float *signal){
      connect_input(0, signal);
    }

    inline void connect_output(int index, float *signal) {
      out[index] = signal;
    }

    inline void connect_output(float *signal){
      connect_output(0, signal);
    }

    inline float get_input(int index) {
      return *in[index];
    }

    inline float get_input() {
      return get_input(0);
    }

    inline float get_output(int index) {
      return *out[index];
    }

    inline float get_output() {
      return get_output(0);
    }

    inline float write_output(int index, float value) {
      *(this->out[index]) = value;
      return value;
    }

    inline float write_output(double value) {
      return write_output(0, value);
    }

    inline float * get_input_signal(int index) {
      return in[index];
    }

    inline float * get_input_signal() {
      return get_input_signal(0);
    }

    inline float * get_output_signal(int index) {
      return out[index];
    }

    inline float * get_output_signal() {
      return get_output_signal(0);
    }

    inline void set_probe(float value) {
      probe_value = value;
    }

    inline float probe() {
      return probe_value;
    }

private:
  float * in[inputs];
  float * out[outputs];
  float probe_value;

};

#endif
