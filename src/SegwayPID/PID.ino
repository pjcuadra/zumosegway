/** K_p value */
const float P = 36;
/** K_i value */
const float I = 576;
/** K_d value */
const float D = 0.5625;

/** Simplification constant a */
const float a = (P + I/(2 * samplingFrequency) + D * samplingFrequency);
/** Simplification constant b */
const float b = (-P + I/(2 * samplingFrequency) - 2 * samplingFrequency * D);
/** Simplification constant c */
const float c = D * samplingFrequency;


/**
 * Discrete PID implementation to control zumo's speed
 */
void pid(){
  static float u_1 = 0;
  static float e_1 = 0;
  static float e_2 = 0;
  
  speed = speed + a * angleError + b * e_1 + c * e_2;
  
  e_2 = e_1;
  e_1 = angleError;
}


