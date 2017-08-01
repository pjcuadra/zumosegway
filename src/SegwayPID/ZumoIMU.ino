#include <Wire.h>
#include <Zumo32U4.h>

L3G gyro;
LSM303 compass;

float a_angle;
float g_read;
float g_angular_speed;
float hp_g_angular_speed;
float c_angle;

// This is the average reading obtained from the gyro's Y axis
// during calibration.
float gyroOffsetY;

// This is just like "angle", but it is based solely on the
// accelerometer.
float aAngle = 0;

void setup_gyro() {
  // Set up the L3GD20H gyro.
  gyro.init();

  // 800 Hz output data rate,
  // low-pass filter cutoff 100 Hz.
  gyro.writeReg(L3G::CTRL1, 0b11111111);

  // 2000 dps full scale.
  gyro.writeReg(L3G::CTRL4, 0b00100000);

  // High-pass filter disabled.
  gyro.writeReg(L3G::CTRL5, 0b00000000);
}

inline void setup_accelerometer() {
  // Set up the LSM303D accelerometer.
  compass.init();

  // 50 Hz output data rate
  compass.writeReg(LSM303::CTRL1, 0x57);

  // 8 g full-scale
  compass.writeReg(LSM303::CTRL2, 0x18);
}

void get_angular_speed() {
  gyro.read();
  g_read = -gyro.g.y;

  g_angular_speed = g_read * 70 / 1000000;
}


// Reads the gyro and uses it to update the angle estimation.
void updateAngleGyro() {
  // Figure out how much time has passed since the last update.
  // Figure out how much time has passed since the last update.
  static uint16_t lastUpdate = 0;
  uint16_t m = micros();
  uint16_t dt = m - lastUpdate;
  lastUpdate = m;

  get_angular_speed();

  // Calculate how much the angle has changed, in degrees, and
  // add it to our estimation of the current angle.  The gyro's
  // sensitivity is 0.07 dps per digit.
  angle += ((float)gyroOffsetY + g_read) * 70 * dt / 1000000000;
}

void get_angle() {
  compass.read();
  a_angle = atan2(compass.a.z, -compass.a.x) * 180 / M_PI;
}

// Reads the accelerometer and uses it to adjust the angle
// estimation.
void correctAngleAccel() {
  get_angle();

  // Calculate the angle according to the accelerometer.
  aAngle = a_angle;

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

  // Adjust the angle estimation.  The higher the weight, the
  // more the angle gets adjusted.
  angle = weight * aAngle + (1 - weight) * angle;
}


void gyro_calibrate() {
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

