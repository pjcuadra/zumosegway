/** Zumo 100:1 motor gear ratio */
const float gearRatio = 100.37;
/** Encoder count to cycle convertion constant */
const float countToDegrees = 360 / (float)(12.0 * gearRatio);

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
  static float prevPosition = 0;
  static uint16_t lastUpdate = 0;
  static float leftPosition = 0;
  static float rightPosition = 0;
  uint16_t m = micros();
  uint16_t dt = m - lastUpdate;
  lastUpdate = m;
  
  leftPosition += (float)encoders.getCountsAndResetLeft() * countToDegrees;
  rightPosition += (float)encoders.getCountsAndResetRight() * countToDegrees;
  float motorAngularPosition = -(leftPosition + rightPosition) / 2.0;
  
  motorAngularSpeed = (motorAngularPosition - prevPosition) * 1000000.0 / dt;
  prevPosition = motorAngularPosition;
}

