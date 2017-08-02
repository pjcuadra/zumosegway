/** Control Law*/
float K[statesNumber] = {0.071609,  9.785357, 0.184250, 0.596573};
/** Radians to Degree convertion constant */
const float radianToDegree = 180.0 / M_PI;
/** Degree to convertion constant */
const float degreeToRadian = 1 / radianToDegree;

const float scaleConst = 1;

/**
 * LQR control law
 */
void lqr() {
  speed = 0;
  
  speed -= scaleConst * motorAngularPosition * K[0];
  speed -= scaleConst * correctedAngularPosition * K[1];
  speed -= scaleConst * motorAngularSpeed * K[2];
  speed -= scaleConst * angularSpeed * K[3];
 
}

