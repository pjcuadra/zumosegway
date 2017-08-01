/** Zumo 100:1 motor gear ratio */
const float gearRatio = 100.37;
/** Encoder count to cycle convertion constant */
const float countToCycles = 1 / (12* gearRatio);
/** Zumo encoders */
Zumo32U4Encoders encoders;

/**
 * Clear the counters of the encoder
 */
void clearEncoders() {
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
}

/**
 * Sample the encoders
 */
void sampleEncoders() {
  // Figure out how much time has passed since the last update.
  static uint16_t lastUpdate = 0;
  uint16_t m = micros();
  uint16_t dt = m - lastUpdate;
  lastUpdate = m;
  
  float leftPosition = encoders.getCountsAndResetLeft() * 2 * M_PI * countToCycles;
  float rightPosition = encoders.getCountsAndResetRight() * 2 * M_PI * countToCycles;
  float avgPosition = (leftPosition + rightPosition) / 2.0;

  
  motorAngularSpeed = avgPosition * dt / 1000000;
  motorAngularPosition += motorAngularSpeed;
}

