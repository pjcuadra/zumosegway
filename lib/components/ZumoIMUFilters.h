/*
 * This component is based on the Balacing example available in the
 * Zumo32U4 library.
 */

#ifndef ZUMOIMUFILTERS_H_
#define ZUMOIMUFILTERS_H_

#ifdef ARDUINO
#include <Arduino.h>
#include <Zumo32U4.h>
#endif

#include<Component.h>
#include<Plotter.h>
#include<Filter.h>
#include<Util.h>

/**
 * Signal Class
 */
class ZumoIMUFilters: public Component {
public:
  /**
   * Constructor
   */
  ZumoIMUFilters() {
    // Set up the L3GD20H gyro.
    gyro.init();

    // 800 Hz output data rate,
    // low-pass filter cutoff 100 Hz.
    gyro.writeReg(L3G::CTRL1, 0b11111111);

    // 2000 dps full scale.
    gyro.writeReg(L3G::CTRL4, 0b00100000);

    // High-pass filter disabled.
    gyro.writeReg(L3G::CTRL5, 0b00000000);

    if (!compass.init()) {
      // Failed to detect the compass.
      ledRed(1);
      while(1) {
        delay(100);
      }
    }

    compass.enableDefault();

    aAngle = 0;
    gyAngle = 0;

    butter_lp_5_hz = new Filter<3>(b_lp_5_hz, a_lp_5_hz);
    butter_hp_5_hz = new Filter<3>(b_hp_5_hz, a_hp_5_hz);

  }

  /**
   * Reads the accelerometer and uses it to adjust the angle estimation.
   */
  void get_compass_angle() {
    compass.read();

    // Calculate the angle according to the accelerometer.
    aAngle = atan2(compass.a.z, -compass.a.x);
  }

  /**
   * Reads the gyro and uses it to update the angle estimation.
   */
  void get_gyro_angle() {
    gyro.read();

    // Calculate how much the angle has changed, in degrees, and
    // add it to our estimation of the current angle.  The gyro's
    // sensitivity is 0.07 dps per digit.
    gyAngle = DEG2RAD(((float) gyro.g.y) * 0.07);
  }

  inline float get_angle() {
    get_compass_angle();
    return butter_lp_5_hz->filter(aAngle);
  }

  inline float get_angular_speed() {
    get_gyro_angle();
    return butter_hp_5_hz->filter(gyAngle);
  }


private:
  /** Gyro */
  L3G gyro;
  /** Compass */
  LSM303 compass;
  float aAngle;
  /** Angle rate read from gyro */
  float gyAngle;
  /** Low-pass filter for accelerometer */
  Filter<3> * butter_lp_5_hz;
  /** High-pass filter for gyro */
  Filter<3> * butter_hp_5_hz;
  /** B coefficients for Low-pass filter (f_c = 5 Hz / f_s = 50 Hz) */
  const float b_lp_5_hz[4] = {0.0028982, 0.0086946, 0.0086946, 0.0028982};
  // const float b_lp_5_hz[4] = {1, 0, 0, 0};
  /** A coefficients for Low-pass filter (f_c = 5 Hz / f_s = 50 Hz) */
  const float a_lp_5_hz[4] = {1.00000, -2.37409, 1.92936, -0.53208};
  // const float a_lp_5_hz[4] = {1, 0, 0, 0};
  /** B coefficients for High-pass filter (f_c = 5 Hz / f_s = 50 Hz) */
  const float b_hp_5_hz[4] = {0.72944, -2.18832, 2.18832, -0.72944};
  // const float b_hp_5_hz[4] = {1, 0, 0, 0};
  /** A coefficients for High-pass filter (f_c = 5 Hz / f_s = 50 Hz) */
  const float a_hp_5_hz[4] = {1.00000, -2.37409, 1.92936, -0.53208};
  // const float a_hp_5_hz[4] = {1, 0, 0, 0};
};

#endif
