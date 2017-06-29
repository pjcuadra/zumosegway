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

/**
 * Signal Class
 */
class ZumoIMUFilters: public Component {
public:
  /** Output port */
  Port out;

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
    angle = 0;

    calibrate();

  }

  /**
   * Print angles
   */
  void printAngles() {
    // Plotter::plot(0, angle.read());
    // Plotter::plot(1, gyAngle);
    // Plotter::plot(2, aAngle);
    // Serial1.println(angle.read());
  }

  /**
   * Calibrate Gyro
   */
  void calibrate() {
    lcd.clear();
    lcd.print(F("Gyro cal"));
    ledYellow(1);

    // Delay to give the user time to remove their finger.
    delay(500);

    angle = 0;
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
    // Calculate the magnitude of the measured acceleration vector,
    // in units of g.
    LSM303::vector<float> const aInG = {
      (float)compass.a.x / 4096,
      (float)compass.a.y / 4096,
      (float)compass.a.z / 4096}
    ;
    float mag = sqrt(LSM303::vector_dot(&aInG, &aInG));

    // Calculate how much weight we should give to the
    // accelerometer reading.  When the magnitude is not close to
    // 1 g, we trust it less because it is being influenced by
    // non-gravity accelerations, so we give it a lower weight.
    float weight = 1 - 5 * abs(1 - mag);
    weight = constrain(weight, 0, 1);
    weight /= 100;
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
    gyAngle = ((float)gyro.g.y - gyroOffsetY) * 70 * dt / 1000000000;
  }

  /**
   * Simluate the component
   */
  inline float simulate() {

    get_compass_angle();
    get_gyro_angle();

    angle = (filter_constant)*(angle.read() + gyAngle) + (1 - filter_constant)*(aAngle);

    // printAngles();

    return out.write(-angle.read());
  }


private:
  /** Gyro */
  L3G gyro;
  /** Compass */
  LSM303 compass;
  /** LCD */
  Zumo32U4LCD lcd;
  /** Angle signal */
  Signal angle;
  /** Average reading obtained from the gyro's Y axis during calibration. */
  double gyroOffsetY;
  /** This is just like "angle", but it is based solely on the accelerometer. */
  float aAngle;
  /** This is just like "angle", but it is based solely on the gyro. */
  float gyAngle;
  const float filter_constant = 0.95;
};

#endif
