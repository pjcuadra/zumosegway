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

Signal in_0;
Signal in_1;
Signal out_0;
Signal out_1;
const float delta = 0.0001;
const int integer_limits = 5;
const float floating_limits = 1;
Integral integ(-integer_limits, integer_limits);
Integral integf(-floating_limits, floating_limits);


void setUp(void) {
  // set stuff up here
  integ.out = out_0;
  integ.in = in_0;
  integf.out = out_1;
  integf.in = in_1;

}

// void tearDown(void) {
// // clean stuff up here
// }

void integrate_integer(void) {
  // Test that inputs/outputs are connected
  TEST_ASSERT_EQUAL_FLOAT(integ.out.read(), 0);
  TEST_ASSERT_EQUAL_FLOAT(integ.in.read(), 0);

  integ.simulate();

  TEST_ASSERT_EQUAL_FLOAT(integ.out.read(), 0);
  TEST_ASSERT_EQUAL_FLOAT(integ.in.read(), in_0.read());

  // Change input signal and simulate
  in_0 = 1;
  TEST_ASSERT_EQUAL_FLOAT(integ.out.read(), 0);
  TEST_ASSERT_EQUAL_FLOAT(integ.in.read(), in_0.read());

  // Intergate for integer_limits simulation times
  for (int i = 1; i <= integer_limits; i++) {
    integ.simulate();
    TEST_ASSERT_EQUAL_FLOAT(integ.out.read(), i);
    TEST_ASSERT_EQUAL_FLOAT(integ.in.read(), in_0.read());
  }

  // Try to integrate above the upper limit
  for (int i = 0; i < integer_limits; i++) {
    integ.simulate();
    TEST_ASSERT_EQUAL_FLOAT(integ.out.read(), integer_limits);
    TEST_ASSERT_EQUAL_FLOAT(integ.in.read(), in_0.read());
  }

  in_0 = -1;
  TEST_ASSERT_EQUAL_FLOAT(integ.out.read(), integer_limits);
  TEST_ASSERT_EQUAL_FLOAT(integ.in.read(), in_0.read());

  for (int i = integer_limits; i >= -integer_limits; i--) {
    TEST_ASSERT_EQUAL_FLOAT(integ.in.read(), in_0.read());
    TEST_ASSERT_EQUAL_FLOAT(integ.out.read(), i);
    integ.simulate();
  }

  // Try to integrate below the lower limit
  for (int i = 0; i < integer_limits; i++) {
    integ.simulate();
    TEST_ASSERT_EQUAL_FLOAT(integ.out.read(), -integer_limits);
    TEST_ASSERT_EQUAL_FLOAT(integ.in.read(), in_0.read());
  }
}

void integrate_floating(void) {
  in_1 = 0;
  // Test that inputs/outputs are connected
  TEST_ASSERT_EQUAL_FLOAT(integf.out.read(), 0);
  TEST_ASSERT_EQUAL_FLOAT(integf.in.read(), in_1.read());

  integf.simulate();

  TEST_ASSERT_EQUAL_FLOAT(integf.out.read(), 0);
  TEST_ASSERT_EQUAL_FLOAT(integf.in.read(), in_1.read());

  // Change input signal and simulate
  in_1 = 0.1;
  TEST_ASSERT_EQUAL_FLOAT(integf.out.read(), 0);
  TEST_ASSERT_EQUAL_FLOAT(integf.in.read(), in_1.read());

  // Intergate until saturation
  for (float i = in_1.read(); i <= floating_limits; i += in_1.read()) {
    integf.simulate();
    TEST_ASSERT_EQUAL_FLOAT(integf.out.read(), i);
    TEST_ASSERT_EQUAL_FLOAT(integf.in.read(), in_1.read());
  }

  // Try to integrate above the upper limit
  for (float i = 0; i < floating_limits; i += in_1.read()) {
    integf.simulate();
    TEST_ASSERT_EQUAL_FLOAT(integf.out.read(), floating_limits);
    TEST_ASSERT_EQUAL_FLOAT(integf.in.read(), in_1.read());
  }

  in_1 = -1;
  TEST_ASSERT_EQUAL_FLOAT(integf.out.read(), floating_limits);
  TEST_ASSERT_EQUAL_FLOAT(integf.in.read(), in_1.read());

  // Intergate until saturation
  for (float i = floating_limits; i >= -floating_limits; i += in_1.read()) {
    TEST_ASSERT_EQUAL_FLOAT(integf.out.read(), i);
    TEST_ASSERT_EQUAL_FLOAT(integf.in.read(), in_1.read());
    integf.simulate();
  }

  // Try to integrate below the lower limit
  for (float i = 0; i > -floating_limits; i += in_1.read()) {
    integf.simulate();
    TEST_ASSERT_EQUAL_FLOAT(integf.out.read(), -floating_limits);
    TEST_ASSERT_EQUAL_FLOAT(integf.in.read(), in_1.read());
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
