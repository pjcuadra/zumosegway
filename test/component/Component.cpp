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
#include <Component.h>

float in_0 = 0;
float in_1 = 1;
float out_0 = 2;
float out_1 = 3;
Component<2, 2> cmp;

void input_assignment(void) {

  cmp.connect_input(&in_0);

  TEST_ASSERT_EQUAL(cmp.get_input(), in_0);
  TEST_ASSERT_EQUAL(cmp.get_input(0), in_0);
  TEST_ASSERT_EQUAL(cmp.get_input_signal(), &in_0);
  TEST_ASSERT_EQUAL(cmp.get_input_signal(0), &in_0);

  cmp.connect_input(0, &in_0);

  TEST_ASSERT_EQUAL(cmp.get_input(), in_0);
  TEST_ASSERT_EQUAL(cmp.get_input(0), in_0);
  TEST_ASSERT_EQUAL(cmp.get_input_signal(), &in_0);
  TEST_ASSERT_EQUAL(cmp.get_input_signal(0), &in_0);


  cmp.connect_input(1, &in_1);

  TEST_ASSERT_EQUAL(cmp.get_input(), in_0);
  TEST_ASSERT_EQUAL(cmp.get_input(0), in_0);
  TEST_ASSERT_EQUAL(cmp.get_input(1), in_1);
  TEST_ASSERT_EQUAL(cmp.get_input_signal(), &in_0);
  TEST_ASSERT_EQUAL(cmp.get_input_signal(0), &in_0);
  TEST_ASSERT_EQUAL(cmp.get_input_signal(1), &in_1);

}


void output_assignment(void) {

  cmp.connect_output(&out_0);

  TEST_ASSERT_EQUAL(cmp.get_output(), out_0);
  TEST_ASSERT_EQUAL(cmp.get_output(0), out_0);
  TEST_ASSERT_EQUAL(cmp.get_output_signal(), &out_0);
  TEST_ASSERT_EQUAL(cmp.get_output_signal(0), &out_0);

  cmp.connect_output(0, &out_0);

  TEST_ASSERT_EQUAL(cmp.get_output(), out_0);
  TEST_ASSERT_EQUAL(cmp.get_output(0), out_0);
  TEST_ASSERT_EQUAL(cmp.get_output_signal(), &out_0);
  TEST_ASSERT_EQUAL(cmp.get_output_signal(0), &out_0);

  cmp.connect_output(1, &out_1);

  TEST_ASSERT_EQUAL(cmp.get_output(), out_0);
  TEST_ASSERT_EQUAL(cmp.get_output(0), out_0);
  TEST_ASSERT_EQUAL(cmp.get_output(1), out_1);
  TEST_ASSERT_EQUAL(cmp.get_output_signal(), &out_0);
  TEST_ASSERT_EQUAL(cmp.get_output_signal(0), &out_0);
  TEST_ASSERT_EQUAL(cmp.get_output_signal(1), &out_1);

}

void probe(void) {

  cmp.set_probe(5);
  TEST_ASSERT_EQUAL(cmp.probe(), 5);

}

void output_write(void) {
  cmp.write_output(6);

  TEST_ASSERT_EQUAL(cmp.get_output(), 6);
  TEST_ASSERT_EQUAL(cmp.get_output(0), 6);
  TEST_ASSERT_EQUAL(cmp.get_output(1), 0);

  cmp.write_output(0, 7);

  TEST_ASSERT_EQUAL(cmp.get_output(), 7);
  TEST_ASSERT_EQUAL(cmp.get_output(0), 7);
  TEST_ASSERT_EQUAL(cmp.get_output(1), 0);

  cmp.write_output(1, 8);

  TEST_ASSERT_EQUAL(cmp.get_output(), 7);
  TEST_ASSERT_EQUAL(cmp.get_output(0), 7);
  TEST_ASSERT_EQUAL(cmp.get_output(1), 8);

}

void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  UNITY_BEGIN();

  RUN_TEST(output_write);
  RUN_TEST(probe);
  RUN_TEST(output_assignment);
  RUN_TEST(input_assignment);

}

void loop() {
  UNITY_END();
}


#endif
