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

#ifdef ARDUINO
#include <Arduino.h>
#endif
#include <unity.h>
#include <Adder.h>

#ifdef UNIT_TEST

Signal in_0;
Signal in_1;
Signal out;
Adder adder;

void setUp(void) {
  // set stuff up here
  adder.out = out;
  adder.in_0 = in_0;
  adder.in_1 = in_1;
}

void adder_component(void) {
  // Test that inputs/outputs are connected
  TEST_ASSERT_EQUAL(adder.out.read(), 0);
  TEST_ASSERT_EQUAL(adder.in_0.read(), 0);
  TEST_ASSERT_EQUAL(adder.in_1.read(), 0);

  // Change input signal and simulate
  in_0 = 4;
  TEST_ASSERT_EQUAL(adder.in_0.read(), 4);
  TEST_ASSERT_EQUAL(adder.in_1.read(), 0);
  TEST_ASSERT_EQUAL(adder.out.read(), 0);
  adder.simulate();
  TEST_ASSERT_EQUAL(adder.out.read(), 4);

  // Change the other input signal and simulate
  in_1 = 3;
  TEST_ASSERT_EQUAL(adder.in_0.read(), 4);
  TEST_ASSERT_EQUAL(adder.in_1.read(), 3);
  TEST_ASSERT_EQUAL(adder.out.read(), 4);
  adder.simulate();
  TEST_ASSERT_EQUAL(adder.out.read(), 7);

  // Simulate again to verify output isn't accumulating

  adder.simulate();
  TEST_ASSERT_EQUAL(adder.out.read(), 7);

}

#ifdef ARDUINO

  void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    UNITY_BEGIN();

    RUN_TEST(adder_component);

  }

  void loop() {
    UNITY_END();
  }

#else

  int main(int argc, char **argv) {
      // run_all_tests();
      return 0;
  }

#endif

#endif
