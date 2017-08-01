inline void digital_filter(const int degree, const float *a, const float *b, float *x, float *y, float in, float &out) {

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
 * FIR Filter function
 * @param degree filter's degree
 * @param c
 */
inline void fir_filter(const int degree, const float *c, float *x, float in, float &out) {
  float tmp_out = 0;
  
  // Apply filter's coeficcients
  for (int i = 0; i < degree; i++){
    tmp_out += c[i] * x[i];
  }
  
  out = tmp_out;
}

