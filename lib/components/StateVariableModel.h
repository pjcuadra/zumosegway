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

#ifndef STATE_VARIABLE_MODEL_H_
#define STATE_VARIABLE_MODEL_H_

#include<Component.h>
#include<Gain.h>
#include<Adder.h>
#include<Integral.h>
#include<Limit.h>
#include<Derivative.h>

/**
 * State Variable Component
 */
template<int num_states, int num_inputs, int num_output>
class StateVariableModel: public Component {
public:
  /** Input port */
  Port in[num_inputs];
  /** Output port */
  Port out[num_output];

  /** Input Signal */
  Signal s_in;

  Signal state_vector[num_states];

  /**
   * Simulate the circuit component
   *
   *  x' = Ax + Bu
   *   y = Cx + Du
   *
   * @param A A state variable model matrix
   * @param B B state variable model matrix
   * @param C C state variable model matrix
   * @param D D state variable model matrix
   */
  StateVariableModel(
    double A[num_states][num_states],
    double B[num_states][num_inputs],
    double C[num_output][num_states],
    double D[num_output][num_inputs]) {

    for (int i = 0; i < num_states; i++) {
      // Copy A
      for (int j = 0; j < num_states; j++) {
        this->A[i][j] = A[i][j];
      }

      // Copy B
      for (int j = 0; j < num_inputs; j++) {
        this->B[i][j] = B[i][j];
      }

      // Copy C
      for (int j = 0; j < num_output; j++) {
        this->C[j][i] = C[j][i];
      }
    }

    // Copy D
    for (int i = 0; i < num_output; i++) {
      for (int j = 0; j < num_states; j++) {
        this->D[i][j] = D[i][j];
      }
    }

    // Implement derivative
    for (int i = 0; i < num_states; i++) {
      this->A[i][i] += freq;
    }

  }

  /**
   * Simulate the circuit component
   */
  inline double simulate() {
    double tmp_out = 0;

    // Update the state
    for (int i = 0; i < num_states; i++){

      // Sum up the derivative of the state
      for (int j = 0; j < num_states; j++){
        state_vector[i] += A[i][j] * state_vector[i];
      }

      // Sum up the inputs
      for (int j = 0; j < num_inputs; j++){
        state_vector[i] += B[i][j] * in[j].read();
      }
    }

    // Update the output
    for (int i = 0; i < num_output; i++){

      tmp_out = 0;

      // Sum up the derivative of the state
      for (int j = 0; j < num_states; j++){
        tmp_out += C[i][j] * state_vector[j];
      }

      // Sum up the inputs
      for (int j = 0; j < num_inputs; j++){
        tmp_out += D[i][j] * in[j].read();
      }

      out[i].write(tmp_out);
    }
  }

private:
  /** A matrix */
  double A[num_states][num_states];
  /** B matrix */
  double B[num_states][num_inputs];
  /** C matrix */
  double C[num_output][num_states];
  /** D matrix */
  double D[num_output][num_inputs];

};

#endif
