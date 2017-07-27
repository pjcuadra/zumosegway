Overview
========

Complementary Filters
---------------------

The name Complementary is realized due to the presence of both LPF(Low Pass Filter) and HPF(High Pass Filter).
We consider the following framework to deploy *IMU* sensory-model results for balancing *Segway*;

Gyroscope
+++++++++

We have implemented Butterworth High pass Filter with following considerations;

.. math::
* Cut-off frequency = 5 Hz
* Order = Third
* Sampling Rate = 50 Hz

.. code::

  /** High-pass filter for gyro */
  Filter<3> * butter_hp_5_hz;

  /** B coefficients for High-pass filter (f_c = 5 Hz / f_s = 50 Hz) */
  const double b_hp_5_hz[4] = {0.72944, -2.18832,  2.18832, -0.72944};
  /** A coefficients for High-pass filter (f_c = 5 Hz / f_s = 50 Hz) */
  const double a_hp_5_hz[4] = {1.00000, -2.37409,  1.92936, -0.53208};


Accelerometer
+++++++++++++

We have implemented Butterworth Low pass Filter with following considerations;

.. math::
* Cut-off frequency = 5 Hz
* Order = Third
* Sampling Rate = 50 Hz

.. code::

  /** Low-pass filter for accelerometer */
  Filter<3> * butter_lp_5_hz;

  /** B coefficients for Low-pass filter (f_c = 5 Hz / f_s = 50 Hz) */
  const double b_lp_5_hz[4] = {0.0028982, 0.0086946, 0.0086946, 0.0028982};
  /** A coefficients for Low-pass filter (f_c = 5 Hz / f_s = 50 Hz) */
  const double a_lp_5_hz[4] = {1.00000, -2.37409, 1.92936, -0.53208};

.. note::
* Gyroscope (HPF) output is integrated and added to Accelerometer to get the exact location
