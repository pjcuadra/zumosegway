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

#ifndef STATE_FEEDBACK_MODEL_H_
#define STATE_FEEDBACK_MODEL_H_

#include<Component.h>
#include<Gain.h>
#include<Adder.h>
#include<Integral.h>
#include<Limit.h>
#include<Derivative.h>

/**
 * State Feedback Component
 */
template<int num_states, int num_inputs>
  class StateFeedback: public Component {
  public:
    /** Input port */
    Port in[num_states];
    /** Output port */
    Port out[num_inputs];

    /**
     * Simulate the circuit component
     * @param K control law vector
     */
    StateFeedback(const double K[num_states]) {
      for (int i = 0; i < num_states; i++) {
          this->K[i] = K[i];
      }
    }

    /**
     * Simulate the circuit component
     */
    inline double simulate() {
      double tmp_out = 0;

      // Update the state
      for (int i = 0; i < num_inputs; i++) {
        // Init
        tmp_out = 0;

        // Perform vector multiplication
        for (int j = 0; j < num_states; j++){
          tmp_out += in[j].read() * K[j];
        }

        out[i].write(tmp_out);

      }

      return 0;
    }

  private:
    /** Control law. */
    double K[num_states];

  };

#endif
