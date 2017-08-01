/** Control Law*/
float K[statesNumber] = {3.1623, 500.2828, 1.8826, 100.3347};
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
  speed -= scaleConst * correctedAngularPosition * degreeToRadian * K[1];
  speed -= scaleConst * motorAngularSpeed * K[2];
  speed -= scaleConst * angularSpeed * degreeToRadian * K[3];
 
}

