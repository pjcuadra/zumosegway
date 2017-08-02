#include <Wire.h>
#include <Zumo32U4.h>

/** Acceleromenter angle */
float accelerometerAngle = 0;
/** Zumos Gyro */
L3G gyro;
/** Zumos Accelerometer */
LSM303 compass;
/** Gyro's bias */
float gyroOffsetY;

/**
 * Setup the Gyro and Accelerometer
 */
void setupIMU() {
  // Set up the L3GD20H gyro.
  gyro.init();

  // 800 Hz output data rate,
  // low-pass filter cutoff 100 Hz.
  gyro.writeReg(L3G::CTRL1, 0b11111111);

  // 2000 dps full scale.
  gyro.writeReg(L3G::CTRL4, 0b00100000);

  // High-pass filter disabled.
  gyro.writeReg(L3G::CTRL5, 0b00000000);

    // Set up the LSM303D accelerometer.
  compass.init();

  // 50 Hz output data rate
  compass.writeReg(LSM303::CTRL1, 0x57);

  // 8 g full-scale
  compass.writeReg(LSM303::CTRL2, 0x18);
}

/**
 * Reads the Gyro changing rate and integrate it adding it to the angle
 */
void sampleGyro() {
  // Figure out how much time has passed since the last update.
  static uint16_t lastUpdate = 0;
  uint16_t m = micros();
  uint16_t dt = m - lastUpdate;
  float gyroAngularSpeed = 0;
  lastUpdate = m;

  gyro.read();
  gyroAngularSpeed = ((float)gyroOffsetY - (float)gyro.g.y) * 70 / 1000.0;

  // Calculate how much the angle has changed, in degrees, and
  // add it to our estimation of the current angle.  The gyro's
  // sensitivity is 0.07 dps per digit.
  angularPosition += gyroAngularSpeed * dt / 1000000.0;
}

void filterAngularSpeed() {
  const int degree = 3;
  const float a[degree + 1] = {1.0000 ,  -2.3741 ,   1.9294,   -0.5321};
  const float b[degree + 1] = {0.0029  ,  0.0087  ,  0.0087 ,   0.0029};
  static float x[degree + 1] = {0, 0, 0, 0};
  static float y[degree + 1] = {0, 0, 0, 0};

  digitalFilter(degree, a, b, x, y, angularSpeed, angularSpeed);
}

/**
 * Read the acceleormeter and adjust the angle
 */
void sampleAccelerometer() {
  static uint16_t lastUpdate = 0;
  uint16_t m = micros();
  uint16_t dt = m - lastUpdate;
  float gyroAngularSpeed = 0;
  static float prevAngularPosition = 0;
  lastUpdate = m;

  compass.read();
  accelerometerAngle = atan2(compass.a.z, -compass.a.x) * 180 / M_PI;

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
  weight /= 10;

  prevAngularPosition = angularPosition;

  // Adjust the angle estimation.  The higher the weight, the
  // more the angle gets adjusted.
  angularPosition = weight * accelerometerAngle + (1 - weight) * angularPosition;
  angularSpeed = (angularPosition - prevAngularPosition) * 1000000.0 / dt;
}

/**
 * Calibrante the Gyroscope, to get the bias.
 */
void calibrateGyro() {
  ledYellow(1);

  // Delay to give the user time to remove their finger.
  delay(500);

  // Calibrate the gyro.
  for (uint16_t i = 0; i < 1024; i++) {
    // Wait for new data to be available, then read it.
    while(!gyro.readReg(L3G::STATUS_REG) & 0x08);
    gyro.read();

    // Add the Y axis reading to the total.
    gyroOffsetY += gyro.g.y;
  }
  gyroOffsetY /= 1024;

  ledYellow(0);
}

