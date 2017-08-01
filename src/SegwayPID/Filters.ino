/**
 * Generic Digital filter function
 * @param degree is the degree of the filter
 * @param a array of size degree + 1 with the denominator's coefficients 
 * @param b array of size degree + 1 with the numerator's coefficients
 * @param x array of size degree + 1 to store the previous input states
 * @param y array of size degree + 1 to store the previous output states
 * @param in input to the filter
 * @param out filtered output
 */
void digitalFilter(const int degree, const float *a, const float *b, float *x, float *y, float in, float &out) {

  float tmp_out = in * b[0];

  // Apply filter's coeficcients
  for (int i = 0; i < degree; i++){
    tmp_out += b[i + 1] * x[i] - a[i + 1] * y[i];
  }

  // Shift states
  for (int i = degree - 1; i > 0; i--){
    x[i] = x[i - 1];
    y[i] = y[i - 1];
  }

  x[0] = in;
  y[0] = tmp_out/a[0];

  out = y[0];
}

/**
 * FIR Filter function (WIP)
 * @param degree is the degree of the filter
 * @param c array of size degree with the filter's coefficients 
 * @param x array of size degree to store the previous input states
 * @param in input to the filter
 * @param out filtered output
 */
inline void filterFIR(const int degree, const float *c, float *x, float in, float &out) {
  float tmp_out = 0;
  
  // Apply filter's coeficcients
  for (int i = 0; i < degree; i++){
    tmp_out += c[i] * x[i];
  }
  
  out = tmp_out;
}

