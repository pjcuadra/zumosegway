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

/**
 * Component Abstract Class Template
 */
template<int inputs, int outputs>
  /**
   * Component Abstract Class
   */
  class Component {
  public:

    /**
     * Constructor
     */
    Component() {
      // Variables are created for the outputs as placeholders for preventing
      // segmentation faults!
      for (int i = 0; i < outputs; i++) {
        out[i] = new float(0);
      }
    }

    /**
     * Connect to an indexed input of the component
     * @param index index of the input
     * @param signal pointer to the variable of the input
     */
    inline void connect_input(int index, float *signal) {
      in[index] = signal;
    }

    /**
     * Connect to the first input of the component
     * @param signal pointer to the variable of the input
     */
    inline void connect_input(float *signal){
      connect_input(0, signal);
    }

    /**
     * Connect to an indexed output of the component
     * @param index index of the output
     * @param signal pointer to the variable of the output
     */
    inline void connect_output(int index, float *signal) {
      out[index] = signal;
    }

    /**
     * Connect to an indexed output of the component
     * @param index index of the output
     * @param signal pointer to the variable of the output
     */
    inline void connect_output(float *signal){
      connect_output(0, signal);
    }

    /**
     * Get the value of an indexed input of the component
     * @param index index of the input
     * @return value of the input
     */
    inline float get_input(int index) {
      return *in[index];
    }

    /**
     * Get the value of the first input of the component
     * @return value of the input
     */
    inline float get_input() {
      return get_input(0);
    }

    /**
     * Get the value of an indexed output of the component
     * @param index index of the output
     * @return value of the output
     */
    inline float get_output(int index) {
      return *out[index];
    }

    /**
     * Get the value of the first output of the component
     * @return value of the output
     */
    inline float get_output() {
      return get_output(0);
    }

    /**
     * Write the value of an indexed output of the component
     * @param index index of the output
     * @return written value of the output
     */
    inline float write_output(int index, float value) {
      *(this->out[index]) = value;
      return value;
    }

    /**
     * Write the value of the first output of the component
     * @param index index of the output
     * @return written value of the output
     */
    inline float write_output(double value) {
      return write_output(0, value);
    }

    /**
     * Get the pointer to an indexed input
     * @param index index of the input
     * @return pointer to the variable
     */
    inline float * get_input_signal(int index) {
      return in[index];
    }

    /**
     * Get the pointer to the first input
     * @param index index of the input
     * @return pointer to the variable
     */
    inline float * get_input_signal() {
      return get_input_signal(0);
    }

    /**
     * Get the pointer to an indexed output
     * @param index index of the output
     * @return pointer to the variable
     */
    inline float * get_output_signal(int index) {
      return out[index];
    }

    /**
     * Get the pointer to the first output
     * @return pointer to the variable
     */
    inline float * get_output_signal() {
      return get_output_signal(0);
    }

    /**
     * Set the value of an internal probe variable (for debug)
     * @param value value to be stored
     */
    inline void set_probe(float value) {
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
  /** Inputs array */
  float * in[inputs];
  /** Outputs array */
  float * out[outputs];
  /** Probe Variable */
  float probe_value;

};

#endif
