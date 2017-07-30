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

#ifdef NNNN

// Misc.
#include <ArduinoCode.h>
#include <Util.h>
#include <IMU.h>

// ZumoIMUFilters * imu_filters;
// ZumoIMU * imu;
// Signal imu_angle;
// Signal imu_filters_angle;
// Signal imu_filters_angular_speed;
// Signal imu_filters_combined_angle;

/** Sampling period in ms */
const byte sampling_period = 20;
/** Sampling frequency */
const float sampling_freq = 1 / ((float) sampling_period / 1000.0);

void IMU::setup() {

  integral = new Integral(-45, 45, sampling_freq);

  // Print contantas
  plotter->info("Sampling Frequency", sampling_freq);

  // Configure the plots
  plotter->config_plot(0, "title:{IMU Angle}");
  plotter->config_plot(0, "period:{20}");

  plotter->config_plot(1, "title:{IMU Filter Angle}");
  plotter->config_plot(1, "period:{20}");

  plotter->config_plot(2, "title:{IMU Filter Angular Speed}");
  plotter->config_plot(2, "period:{20}");

  plotter->config_plot(3, "title:{IMU Filter Combined Angle}");
  plotter->config_plot(3, "period:{20}");

  startup_wait();
}

void IMU::loop() {
  imu.updateAngleGyro();

  byte current_time = millis();
  if ((byte)(current_time - last_sampled_time) >= sampling_period) {

    last_sampled_time = current_time;
    simulate_circuit();
  }
}

/**
 * Simluate the entire circuit
 */
void IMU::simulate_circuit() {
  float tmp = 0;
  plotter->plot(0, imu.get_angle());
  plotter->plot(1, RAD2DEG(imu_filters.get_angle()));
  plotter->plot(2, RAD2DEG(imu_filters.get_angular_speed()));

  tmp = integral->calculate(imu_filters.get_angular_speed()) + imu_filters.get_angle();

  plotter->plot(3, tmp);
}

#endif
