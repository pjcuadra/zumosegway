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
#include <Derivative.h>

float in = 0;
float out = 0;
Derivative diff;

void setUp(void) {
  // set stuff up here
  diff.connect_output(&out);
  diff.connect_input(&in);

}

// void tearDown(void) {
// // clean stuff up here
// }

void derivative_component(void) {
  // Test that inputs/outputs are connected
  TEST_ASSERT_EQUAL(diff.get_output(), 0);
  TEST_ASSERT_EQUAL(diff.get_input(), 0);

  // Change input signal and simulate
  in = 4;
  TEST_ASSERT_EQUAL(diff.get_input(), 4);
  TEST_ASSERT_EQUAL(diff.get_output(), 0);
  diff.simulate();
  TEST_ASSERT_EQUAL(diff.get_output(), 4);

  in = 7;
  TEST_ASSERT_EQUAL(diff.get_input(), 7);
  TEST_ASSERT_EQUAL(diff.get_output(), 4);
  diff.simulate();
  TEST_ASSERT_EQUAL(diff.get_output(), 3);

  TEST_ASSERT_EQUAL(diff.get_input(), 7);
  TEST_ASSERT_EQUAL(diff.get_output(), 3);
  diff.simulate();
  TEST_ASSERT_EQUAL(diff.get_output(), 0);

}

void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  UNITY_BEGIN();

  RUN_TEST(derivative_component);

}

void loop() {
  UNITY_END();
}


#endif
