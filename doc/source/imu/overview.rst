Overview
========

Inertial Management Unit
------------------------

The Zumo 32U4 includes on-board inertial sensors that can be used in advanced applications, such as helping our Zumo detect collisions and determine its own orientation by implementing an inertial measurement unit (IMU).

.. note::
We used *IMU* as the main sensory-model for *Segway*.

We took the aid of following sensor components of *IMU*;

* Gyroscope : *ST L3GD20H* 3-axis gyroscope. 
* Accelerometer : *ST LSM303D* compass module, which combines a 3-axis accelerometer and 3-axis magnetometer.

.. note::
* Both sensor chips share an I²C bus connected to the ATmega32U4’s I²C interface.
* Level shifters built into the main board allow the inertial sensors, which operate at 3.3 V, to be connected to the ATmega32U4 (operating at 5 V).

Gyroscope
+++++++++

We consider the following aspects of Gyroscope for *IMU* sensory-model;

* Gyroscope provides the change in orientation of the zumo (Roll, Yaw, Pitch). Integration of result provides the position details.
* *ST L3GD20H* Gyroscope operation is based on angular momentum.

Accelerometer
+++++++++++++

We consider the following aspects of Accelerometer for *IMU* sensory-model;

* *ST LSM303D* Accelerometer provides the linear acceleration based on vibration.
* By virtue of linear acceleration, Accelerometer provides 3-dimensional position (X-,Y-,Z- axis)

.. note::
* Gyroscope gives angular position but has tendency to drift over the period of time. Accelerometer gives Inertia, and ultimately position but it is slow. Hence, Accelerometer output is used to correct position obtained from Gyroscope on periodic interval of time.

.. code::

  /**
   * Reads the accelerometer and uses it to adjust the angle estimation.
   */
  void correctAngleAccel() {
    compass.read();

    // Calculate the angle according to the accelerometer.
    aAngle = -atan2(compass.a.z, -compass.a.x) * 180 / M_PI;

    // Calculate the magnitude of the measured acceleration vector,
    // in units of g.
    LSM303::vector<double> const aInG = {
      (double)compass.a.x / 4096,
      (double)compass.a.y / 4096,
      (double)compass.a.z / 4096}
    ;
    double mag = sqrt(LSM303::vector_dot(&aInG, &aInG));

    // Calculate how much weight we should give to the
    // accelerometer reading.  When the magnitude is not close to
    // 1 g, we trust it less because it is being influenced by
    // non-gravity accelerations, so we give it a lower weight.
    double weight = 1 - 5 * abs(1 - mag);
    weight = constrain(weight, 0, 1);
    weight /= 100;

    // Adjust the angle estimation.  The higher the weight, the
    // more the angle gets adjusted.
    angle = weight * aAngle + (1 - weight) * angle.read();
  }
