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
#include <Limit.h>

float in = 0;
Limit lim_0(-0.5, 0.5);
Limit lim_1(-2, 2);
Limit lim_2(3, 4);
Limit lim_3(-3, -1);

void setUp(void) {
  // set stuff up here
  lim_0.connect_input(&in);
  lim_1.connect_input(&in);
  lim_2.connect_input(&in);
  lim_3.connect_input(&in);
}

// void tearDown(void) {
// // clean stuff up here
// }

void limit_component(void) {
  // Test that inputs/outputs are connected
  TEST_ASSERT_EQUAL(lim_0.get_input(), 0);
  TEST_ASSERT_EQUAL(lim_1.get_input(), 0);
  TEST_ASSERT_EQUAL(lim_2.get_input(), 0);
  TEST_ASSERT_EQUAL(lim_3.get_input(), 0);
  TEST_ASSERT_EQUAL(lim_0.get_output(), 0);
  TEST_ASSERT_EQUAL(lim_1.get_output(), 0);
  TEST_ASSERT_EQUAL(lim_2.get_output(), 0);
  TEST_ASSERT_EQUAL(lim_3.get_output(), 0);

  lim_0.simulate();
  lim_1.simulate();
  lim_2.simulate();
  lim_3.simulate();

  // Test only possitive or negative saturation
  in = 0;

  TEST_ASSERT_EQUAL(lim_0.get_output(), 0);
  TEST_ASSERT_EQUAL(lim_1.get_output(), 0);
  TEST_ASSERT_EQUAL(lim_2.get_output(), 3);
  TEST_ASSERT_EQUAL(lim_3.get_output(), -1);

  // Test Floating point
  in = 0.4;

  lim_0.simulate();
  lim_1.simulate();
  lim_2.simulate();
  lim_3.simulate();

  TEST_ASSERT_EQUAL(lim_0.get_output(), 0.4);
  TEST_ASSERT_EQUAL(lim_1.get_output(), 0.4);
  TEST_ASSERT_EQUAL(lim_2.get_output(), 3);
  TEST_ASSERT_EQUAL(lim_3.get_output(), -1);

  // Test positive floating point saturation
  in = 0.6;

  lim_0.simulate();
  lim_1.simulate();
  lim_2.simulate();
  lim_3.simulate();

  TEST_ASSERT_EQUAL(lim_0.get_output(), 0.5);
  TEST_ASSERT_EQUAL(lim_1.get_output(), 0.6);
  TEST_ASSERT_EQUAL(lim_2.get_output(), 3);
  TEST_ASSERT_EQUAL(lim_3.get_output(), -1);

  // Test positive Integer saturation
  in = 3.1;

  lim_0.simulate();
  lim_1.simulate();
  lim_2.simulate();
  lim_3.simulate();

  TEST_ASSERT_EQUAL(lim_0.get_output(), 0.5);
  TEST_ASSERT_EQUAL(lim_1.get_output(), 2);
  TEST_ASSERT_EQUAL(lim_2.get_output(), 3.1);
  TEST_ASSERT_EQUAL(lim_3.get_output(), -1);

  // Test only positive Integer saturation
  in = 5;

  lim_0.simulate();
  lim_1.simulate();
  lim_2.simulate();
  lim_3.simulate();

  TEST_ASSERT_EQUAL(lim_0.get_output(), 0.5);
  TEST_ASSERT_EQUAL(lim_1.get_output(), 2);
  TEST_ASSERT_EQUAL(lim_2.get_output(), 4);
  TEST_ASSERT_EQUAL(lim_3.get_output(), -1);

  // Test negative floating point saturation
  in = -0.6;

  lim_0.simulate();
  lim_1.simulate();
  lim_2.simulate();
  lim_3.simulate();

  TEST_ASSERT_EQUAL(lim_0.get_output(), -0.5);
  TEST_ASSERT_EQUAL(lim_1.get_output(), -0.6);
  TEST_ASSERT_EQUAL(lim_2.get_output(), 3);
  TEST_ASSERT_EQUAL(lim_3.get_output(), -1);

  // Test negative floating point
  in = -1.5;

  lim_0.simulate();
  lim_1.simulate();
  lim_2.simulate();
  lim_3.simulate();

  TEST_ASSERT_EQUAL(lim_0.get_output(), -0.5);
  TEST_ASSERT_EQUAL(lim_1.get_output(), -1.5);
  TEST_ASSERT_EQUAL(lim_2.get_output(), 3);
  TEST_ASSERT_EQUAL(lim_3.get_output(), -1.5);

  // Test negative saturation
  in = -2.1;

  lim_0.simulate();
  lim_1.simulate();
  lim_2.simulate();
  lim_3.simulate();

  TEST_ASSERT_EQUAL(lim_0.get_output(), -0.5);
  TEST_ASSERT_EQUAL(lim_1.get_output(), -2);
  TEST_ASSERT_EQUAL(lim_2.get_output(), 3);
  TEST_ASSERT_EQUAL(lim_3.get_output(), -2.1);


  // Test only negative saturation
  in = -3.1;

  lim_0.simulate();
  lim_1.simulate();
  lim_2.simulate();
  lim_3.simulate();

  TEST_ASSERT_EQUAL(lim_0.get_output(), -0.5);
  TEST_ASSERT_EQUAL(lim_1.get_output(), -2);
  TEST_ASSERT_EQUAL(lim_2.get_output(), 3);
  TEST_ASSERT_EQUAL(lim_3.get_output(), -3);

}

void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  UNITY_BEGIN();

  RUN_TEST(limit_component);

}

void loop() {
  UNITY_END();
}


#endif
