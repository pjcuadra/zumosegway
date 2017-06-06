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

#ifdef UNIT_TEST

#ifdef ARDUINO
#include <Arduino.h>
#endif

#include <unity.h>
#include <PID.h>

void setUp(void) {

}

// void tearDown(void) {
// // clean stuff up here
// }

void pid_p_only(void) {
  const float gain = 2;
  const float it = 13;
  float in = 0;
  PID pid_controller(gain, 0, 0);

  pid_controller.connect_input(&in);

  // Try proportinal with different values
  for(in = 0; in < it; in++) {
    pid_controller.simulate();
    TEST_ASSERT_EQUAL(in * gain, pid_controller.get_output());
  }
}

void pid_i_only(void) {
  const float gain = 2;
  const float it = 13;
  float in = 0;
  float acc = 0;
  PID pid_controller(0, gain, 0);

  pid_controller.connect_input(&in);

  // Try integration by comparing with accumulating
  for(in = 0; in < it; in++) {
    pid_controller.simulate();
    acc += in;
    TEST_ASSERT_EQUAL(acc * gain, pid_controller.get_output());
  }
}

void pid_i_only_limit(void) {

  const float gain = 2;
  const float upper = 5;
  const float it = 13;
  float in = 0;
  float acc = 0;
  PID pid_controller(0, gain, 0, 0, upper);

  pid_controller.connect_input(&in);

  // Try integration by comparing with accumulating within bounds
  for(in = 0; in < it; in++) {
    pid_controller.simulate();
    acc += in;
    if (acc <= upper) {
      TEST_ASSERT_EQUAL(acc * gain, pid_controller.get_output());
    } else {
      TEST_ASSERT_EQUAL(upper * gain, pid_controller.get_output());
    }
  }

}

void pid_d_only(void) {
  const float gain = 2;
  const float it = 13;
  const float inc = 2;
  float in = 0;
  PID pid_controller(0, 0, gain);

  pid_controller.connect_input(&in);

  // Test differential
  for(in = inc; in < it; in+= inc) {
    pid_controller.simulate();
    TEST_ASSERT_EQUAL(inc * gain, pid_controller.get_output());
  }

}

void pid_all(void) {
  const float gain = 1;
  const float it = 3;
  float p = 0;
  float i = 0;
  float d = 0;
  float in = 0;
  PID pid_controller(gain, gain, gain);

  pid_controller.connect_input(&in);

  in = 0;

  pid_controller.simulate();
  p = 0;
  i = 0;
  d = 0;
  TEST_ASSERT_EQUAL(p + i + d, pid_controller.get_output());

  in = 1;

  pid_controller.simulate();
  p = 1 * gain;
  i += 1 * gain;
  d = 1 * gain;
  TEST_ASSERT_EQUAL(p + i + d, pid_controller.get_output());

  // Test integration with constant input (d/dt = 0)
  for(int o = in; o < it; o++) {
    d = 0;
    i += gain;
    pid_controller.simulate();
    TEST_ASSERT_EQUAL(p + i + d, pid_controller.get_output());
  }

  // Change input to -1 (d/dt = -2)
  in = -1;
  pid_controller.simulate();
  p = -1 * gain;
  i += -1 * gain;
  d = -2 * gain;
  TEST_ASSERT_EQUAL(p + i + d, pid_controller.get_output());

  // Test integration with constant input (d/dt = 0) till integral is 0;
  for(int o = in; o < (it - 2); o++) {
    d = 0;
    i -= gain;
    pid_controller.simulate();
    TEST_ASSERT_EQUAL(p + i + d, pid_controller.get_output());
  }

  // Test the integral component to be zero
  TEST_ASSERT_EQUAL(0, i);

  // Change the input to 0 (d/dt = 1)
  in = 0;
  pid_controller.simulate();
  p = 0;
  d = 1 * gain;
  TEST_ASSERT_EQUAL(p + i + d, pid_controller.get_output());

  // Input stays in zero value
  pid_controller.simulate();
  p = 0;
  d = 0;
  TEST_ASSERT_EQUAL(p + i + d, pid_controller.get_output());

}

void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  UNITY_BEGIN();

  RUN_TEST(pid_p_only);
  RUN_TEST(pid_i_only);
  RUN_TEST(pid_i_only_limit);
  RUN_TEST(pid_d_only);
  RUN_TEST(pid_all);

}

void loop() {
  UNITY_END();
}


#endif
