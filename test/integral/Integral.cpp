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
#include <Integral.h>

float in_0 = 0;
float in_1 = 0;
float out_0 = 0;
float out_1 = 0;
const float delta = 0.0001;
const int integer_limits = 5;
const float floating_limits = 1;
Integral integ(-integer_limits, integer_limits);
Integral integf(-floating_limits, floating_limits);


void setUp(void) {
  // set stuff up here
  integ.connect_output(&out_0);
  integ.connect_input(&in_0);
  integf.connect_output(&out_1);
  integf.connect_input(&in_1);

}

// void tearDown(void) {
// // clean stuff up here
// }

void integrate_integer(void) {
  // Test that inputs/outputs are connected
  TEST_ASSERT_EQUAL_FLOAT(integ.get_output(), 0);
  TEST_ASSERT_EQUAL_FLOAT(integ.get_input(), 0);

  integ.simulate();

  TEST_ASSERT_EQUAL_FLOAT(integ.get_output(), 0);
  TEST_ASSERT_EQUAL_FLOAT(integ.get_input(), in_0);

  // Change input signal and simulate
  in_0 = 1;
  TEST_ASSERT_EQUAL_FLOAT(integ.get_output(), 0);
  TEST_ASSERT_EQUAL_FLOAT(integ.get_input(), in_0);

  // Intergate for integer_limits simulation times
  for (int i = 1; i <= integer_limits; i++) {
    integ.simulate();
    TEST_ASSERT_EQUAL_FLOAT(integ.get_output(), i);
    TEST_ASSERT_EQUAL_FLOAT(integ.get_input(), in_0);
  }

  // Try to integrate above the upper limit
  for (int i = 0; i < integer_limits; i++) {
    integ.simulate();
    TEST_ASSERT_EQUAL_FLOAT(integ.get_output(), integer_limits);
    TEST_ASSERT_EQUAL_FLOAT(integ.get_input(), in_0);
  }

  in_0 = -1;
  TEST_ASSERT_EQUAL_FLOAT(integ.get_output(), integer_limits);
  TEST_ASSERT_EQUAL_FLOAT(integ.get_input(), in_0);

  for (int i = integer_limits; i >= -integer_limits; i--) {
    TEST_ASSERT_EQUAL_FLOAT(integ.get_output(), i);
    TEST_ASSERT_EQUAL_FLOAT(integ.get_input(), in_0);
    integ.simulate();
  }

  // Try to integrate below the lower limit
  for (int i = 0; i < integer_limits; i++) {
    integ.simulate();
    TEST_ASSERT_EQUAL_FLOAT(integ.get_output(), -integer_limits);
    TEST_ASSERT_EQUAL_FLOAT(integ.get_input(), in_0);
  }
}

void integrate_floating(void) {
  in_1 = 0;
  // Test that inputs/outputs are connected
  TEST_ASSERT_EQUAL_FLOAT(integf.get_output(), 0);
  TEST_ASSERT_EQUAL_FLOAT(integf.get_input(), in_1);

  integf.simulate();

  TEST_ASSERT_EQUAL_FLOAT(integf.get_output(), 0);
  TEST_ASSERT_EQUAL_FLOAT(integf.get_input(), in_1);

  // Change input signal and simulate
  in_1 = 0.1;
  TEST_ASSERT_EQUAL_FLOAT(integf.get_output(), 0);
  TEST_ASSERT_EQUAL_FLOAT(integf.get_input(), in_1);

  // Intergate until saturation
  for (float i = in_1; i <= floating_limits; i += in_1) {
    integf.simulate();
    TEST_ASSERT_EQUAL_FLOAT(integf.get_output(), i);
    TEST_ASSERT_EQUAL_FLOAT(integf.get_input(), in_1);
  }

  // Try to integrate above the upper limit
  for (float i = 0; i < floating_limits; i += in_1) {
    integf.simulate();
    TEST_ASSERT_EQUAL_FLOAT(integf.get_output(), floating_limits);
    TEST_ASSERT_EQUAL_FLOAT(integf.get_input(), in_1);
  }

  in_1 = -1;
  TEST_ASSERT_EQUAL_FLOAT(integf.get_output(), floating_limits);
  TEST_ASSERT_EQUAL_FLOAT(integf.get_input(), in_1);

  // Intergate until saturation
  for (float i = floating_limits; i >= -floating_limits; i += in_1) {
    TEST_ASSERT_EQUAL_FLOAT(integf.get_output(), i);
    TEST_ASSERT_EQUAL_FLOAT(integf.get_input(), in_1);
    integf.simulate();
  }

  // Try to integrate below the lower limit
  for (float i = 0; i > -floating_limits; i += in_1) {
    integf.simulate();
    TEST_ASSERT_EQUAL_FLOAT(integf.get_output(), -floating_limits);
    TEST_ASSERT_EQUAL_FLOAT(integf.get_input(), in_1);
  }
}

void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  UNITY_BEGIN();

  RUN_TEST(integrate_integer);
  RUN_TEST(integrate_floating);

}

void loop() {
  UNITY_END();
}


#endif
