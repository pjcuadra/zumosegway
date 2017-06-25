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
#include <Gain.h>

Signal in;
Signal out_0;
Signal out_1;
Gain gain_0(5);
Gain gain_1(-1);

void setUp(void) {
  // set stuff up here
  gain_0.out = out_0;
  gain_1.out = out_1;

  gain_0.in = in;
  gain_1.in = in;

}

// void tearDown(void) {
// // clean stuff up here
// }

void gain_component(void) {
  // Test that inputs/outputs are connected
  TEST_ASSERT_EQUAL(gain_0.out.read(), 0);
  TEST_ASSERT_EQUAL(gain_0.in.read(), 0);
  TEST_ASSERT_EQUAL(gain_1.out.read(), 0);
  TEST_ASSERT_EQUAL(gain_1.in.read(), 0);

  gain_0.simulate();
  gain_1.simulate();

  TEST_ASSERT_EQUAL(gain_0.out.read(), 0);
  TEST_ASSERT_EQUAL(gain_0.in.read(), 0);
  TEST_ASSERT_EQUAL(gain_1.out.read(), 0);
  TEST_ASSERT_EQUAL(gain_1.in.read(), 0);

  // Change input signal and simulate
  in = 1;

  TEST_ASSERT_EQUAL(gain_0.out.read(), 0);
  TEST_ASSERT_EQUAL(gain_0.in.read(), 1);
  TEST_ASSERT_EQUAL(gain_1.out.read(), 0);
  TEST_ASSERT_EQUAL(gain_1.in.read(), 1);

  gain_0.simulate();
  gain_1.simulate();

  TEST_ASSERT_EQUAL(gain_0.out.read(), 5);
  TEST_ASSERT_EQUAL(gain_0.in.read(), 1);
  TEST_ASSERT_EQUAL(gain_1.out.read(), -1);
  TEST_ASSERT_EQUAL(gain_1.in.read(), 1);

  // Change input signal and simulate
  in = 2;

  TEST_ASSERT_EQUAL(gain_0.out.read(), 5);
  TEST_ASSERT_EQUAL(gain_0.in.read(), 2);
  TEST_ASSERT_EQUAL(gain_1.out.read(), -1);
  TEST_ASSERT_EQUAL(gain_1.in.read(), 2);

  gain_0.simulate();
  gain_1.simulate();

  TEST_ASSERT_EQUAL(gain_0.out.read(), 10);
  TEST_ASSERT_EQUAL(gain_0.in.read(), 2);
  TEST_ASSERT_EQUAL(gain_1.out.read(), -2);
  TEST_ASSERT_EQUAL(gain_1.in.read(), 2);

}

void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  UNITY_BEGIN();

  RUN_TEST(gain_component);

}

void loop() {
  UNITY_END();
}


#endif
