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

/**
 * Signal Class
 */
class ZumoIMUFilters: public Component {
public:
  /** Angle output port */
  Port angle_out;
  /** Angular speed output port */
  Port angular_speed_out;

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

    butter_lp_5_hz = new Filter<3>(b_lp_5_hz, a_lp_5_hz);
    butter_hp_5_hz = new Filter<3>(b_hp_5_hz, a_hp_5_hz);

    butter_lp_5_hz->in = aAngle;
    butter_lp_5_hz->out = angle_out;

    butter_lp_5_hz->in = gyAngle;
    butter_lp_5_hz->out = angular_speed_out;

    calibrate();

  }

  /**
   * Calibrate Gyro
   */
  void calibrate() {
    ledYellow(1);

    // Delay to give the user time to remove their finger.
    delay(500);

    gyroOffsetY = 0;

    // Calibrate the gyro.
    for (uint16_t i = 0; i < 1024; i++) {
      // Wait for new data to be available, then read it.
      while(!gyro.readReg(L3G::STATUS_REG) & 0x08);
      gyro.read();

      // Add the Y axis reading to the total.
      gyroOffsetY += gyro.g.y;
    }

    gyroOffsetY /= 1024;

    lcd.clear();
    ledYellow(0);
  }

  /**
   * Reads the accelerometer and uses it to adjust the angle estimation.
   */
  void get_compass_angle() {
    compass.read();

    // Calculate the angle according to the accelerometer.
    aAngle = -atan2(compass.a.z, -compass.a.x) * 180 / M_PI;
  }

  /**
   * Reads the gyro and uses it to update the angle estimation.
   */
  void get_gyro_angle() {
    // Figure out how much time has passed since the last update.
    static uint16_t lastUpdate = 0;
    uint16_t m = micros();
    uint16_t dt = m - lastUpdate;
    lastUpdate = m;

    gyro.read();

    // Calculate how much the angle has changed, in degrees, and
    // add it to our estimation of the current angle.  The gyro's
    // sensitivity is 0.07 dps per digit.
    gyAngle.write(((double)gyro.g.y - gyroOffsetY) * 70 * dt / 1000000000);
  }

  /**
   * Simluate the component
   */
  inline double simulate() {

    get_compass_angle();
    get_gyro_angle();

    butter_lp_5_hz->simulate();
    butter_hp_5_hz->simulate();

    // Invert the outputs
    angle_out.write(-angle_out.read());
    angular_speed_out.write(-angular_speed_out.read());

    return 0;
  }


private:
  /** Gyro */
  L3G gyro;
  /** Compass */
  LSM303 compass;
  /** LCD */
  Zumo32U4LCD lcd;
  /** Average reading obtained from the gyro's Y axis during calibration. */
  double gyroOffsetY;
  /** Angle read from accelerometer */
  Signal aAngle;
  /** Angle rate read from gyro */
  Signal gyAngle;
  /** Low-pass filter for accelerometer */
  Filter<3> * butter_lp_5_hz;
  /** High-pass filter for gyro */
  Filter<3> * butter_hp_5_hz;
  /** B coefficients for Low-pass filter (f_c = 5 Hz / f_s = 50 Hz) */
  const double b_lp_5_hz[4] = {0.0028982, 0.0086946, 0.0086946, 0.0028982};
  /** A coefficients for Low-pass filter (f_c = 5 Hz / f_s = 50 Hz) */
  const double a_lp_5_hz[4] = {1.00000, -2.37409, 1.92936, -0.53208};
  /** B coefficients for High-pass filter (f_c = 5 Hz / f_s = 50 Hz) */
  const double b_hp_5_hz[4] = {0.72944, -2.18832,  2.18832, -0.72944};
  /** A coefficients for High-pass filter (f_c = 5 Hz / f_s = 50 Hz) */
  const double a_hp_5_hz[4] = {1.00000, -2.37409,  1.92936, -0.53208};
};

#endif
