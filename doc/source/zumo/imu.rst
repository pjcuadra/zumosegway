.. _r-imu:

Inertial Management Unit
------------------------

The Zumo32U4 includes on-board inertial sensors that can be used in advanced
applications, such as helping our Zumo detect collisions and determine its own
orientation by implementing an inertial measurement unit (IMU).

.. note::

  We used *IMU* as the main sensory-model for *Segway*.

We took the aid of following sensor components of *IMU*;

* Gyroscope : *ST L3GD20H* 3-axis gyroscope.
* Accelerometer : *ST LSM303D* compass module, which combines a 3-axis
  accelerometer and 3-axis magnetometer.

.. note::

  * Both sensor chips share an :math:`I^2C` bus connected to the ATmega32U4’s
    :math:`I^2C` interface.
  * Level shifters built into the main board allow the inertial sensors, which
    operate at 3.3 V, to be connected to the ATmega32U4 (operating at 5 V).

Gyroscope
+++++++++

We consider the following aspects of Gyroscope for *IMU* sensory-model;

* Gyroscope provides the change in orientation of the Zumo (Roll, Yaw, Pitch).
  Integration of result provides the position details.
* *ST L3GD20H* Gyroscope operation is based on angular momentum.
* *ST L3GD20H* provides;
  * Selectable full-scale range of :math:`\pm245dps`/:math:`\pm500dps`/
    :math:`\pm2000dps`, with the :math:`8.75mdps/digit`/:math:`17.5mdps/digit`/
    :math:`70mdps/digit` sensitivity, respectively.
  * Selectable data sampling rate.
  * Low-Pass filter to reduce noise with selectable cut-off frequencies.


.. _gyro_config:
.. code-block:: c
  :caption: Gyroscope configuration :cite:`Example`

  // Set up the L3GD20H gyro.
  gyro.init();

  // 800 Hz output data rate,
  // low-pass filter cutoff 100 Hz.
  gyro.writeReg(L3G::CTRL1, 0b11111010);

  // 2000 dps full scale.
  gyro.writeReg(L3G::CTRL4, 0b00100000);

  // High-pass filter disabled.
  gyro.writeReg(L3G::CTRL5, 0b00000000);

.. note::

  All other register were left with their default value. Review :cite:`STMGYRO`
  for more information regarding default values.

A more detailed descriptions of the configuration used is shown below;

  * :code:`L3G::CTRL1.DR[1:0] = 0x3` selects the :math:`800 Hz` data sampling
    rate :cite:`STMGYRO`.
  * :code:`L3G::CTRL1.BW[1:0] = 0x3` selects :math:`100 Hz` gyroscope data
    cut-off frequency :cite:`STMGYRO`.
  * :code:`L3G::CTRL1.PD = 0x1` selects the normal mode disabling power mode,
    so the signal will be always be sampled :cite:`STMGYRO`.
  * :code:`L3G::CTRL1.XEN = 0x0`, :code:`L3G::CTRL1.YEN = 0x1` and
    :code:`L3G::CTRL1.ZEN = 0x0` enables only the needed gyroscope channel.
  * :code:`L3G::CTRL4.FS[1:0] = 0x2` selects the full-scale of
    :math:`\pm2000dps` with a sensitivity of :math:`70mdps/digit`
    :cite:`STMGYRO`.
  * :code:`L3G::CTRL5.HPen = 0x0` disable the High-Pass filter :cite:`STMGYRO`.

Accelerometer
+++++++++++++

We consider the following aspects of Accelerometer for *IMU* sensory-model;

* *ST LSM303D* Accelerometer provides the linear acceleration based on
  vibration.
* By virtue of linear acceleration, Accelerometer provides 3-dimensional
  position (X-,Y-,Z- axis). :cite:`STMACC`
* *ST LSM303D* provides :math:`\pm2`/:math:`\pm4`/:math:`\pm6`/:math:`\pm8`/
  :math:`\pm16` selectable linear acceleration full-scale. :cite:`STMACC`
* *ST LSM303D* provides :math:`3.125Hz`/:math:`6.25Hz`/:math:`12.5Hz`/
  :math:`25Hz`/:math:`50Hz`/:math:`100Hz`/:math:`200Hz`/:math:`400Hz`/
  :math:`800Hz`/:math:`1600Hz` selectable sampling rate. :cite:`STMACC`

For the implementation of the Segway the sampling frequency, :math:`f_s = 50Hz`,
and full-scale range, :math:`acc_{range} = \pm8g`, were selected. Therefore,
the *ST LSM303D* configuration code is shown in :numref:`acc_config`

.. _acc_config:
.. code-block:: c
  :caption: Compass configuration :cite:`Example`

  // Set up the LSM303D accelerometer.
  compass.init();

  // 50 Hz output data rate
  compass.writeReg(LSM303::CTRL1, 0x57);

  // 8 g full-scale
  compass.writeReg(LSM303::CTRL2, 0x18);

.. note::

  All other register were left with their default value. Review :cite:`STMACC`
  for more information regarding default values.

A more detailed descriptions of the configuration used is shown below;

  * :code:`LSM303::CTRL1.AODR[3:0] = 0x5` sets the :math:`f_s = 50Hz`.
    :cite:`STMACC`
  * :code:`LSM303::CTRL1.BDU = 0x1` enables atomic update for the acceleration
    read register. Meaning that the entire register will be written at once
    :cite:`STMACC`.
  * :code:`LSM303::CTRL1.AXEN = 0x1`, :code:`LSM303::CTRL1.AYEN = 0x1` and
    :code:`LSM303::CTRL1.AZEN = 0x1` enables all three acceleration channels.
    :cite:`STMACC`. All three are needed because the magnitude of the
    acceleration vector is calculated to filter some measurement noise.
    :numref:`angle_correction_acc` shows how the magnitude is used to filter the
    noise.
  * :code:`LSM303::CTRL2.AFS[2:0] = 0x3` sets :math:`acc_{range} = \pm8g`.


Combine Gyroscope and Accelerometer
+++++++++++++++++++++++++++++++++++

Gyroscope gives angular position but has tendency to drift over the period of
time. Accelerometer gives Inertia, and ultimately position but it is slow.
Hence, Accelerometer output is used to correct position obtained from
Gyroscope on periodic interval of time.

First the Gyroscope is being sampled as frequently as possible. Then the data
of the Gyroscope is integrated and to give the current Zumo32u4's angle as
fast as possible. :numref:`gyro_sampling` shows how the sampling and integration
was performed;

.. _gyro_sampling:
.. code-block:: c
  :caption: Gyroscope angle sampling and integration :cite:`Example`

  /** Zumos Gyro */
  L3G gyro;


  /**
  * Reads the Gyro changing rate and integrate it adding it to the angle
  */
  void sampleGyro() {
    // Figure out how much time has passed since the last update.
    static uint16_t lastUpdate = 0;
    uint16_t m = micros();
    uint16_t dt = m - lastUpdate;
    float gyroAngularSpeed = 0;
    lastUpdate = m;

    gyro.read();
    // Obtain the angular speed out of the gyro. The gyro's
    // sensitivity is 0.07 dps per digit.
    gyroAngularSpeed = ((float)gyroOffsetY - (float)gyro.g.y) * 70 / 1000.0;

    // Calculate how much the angle has changed, in degrees, and
    // add it to our estimation of the current angle.
    angularPosition += gyroAngularSpeed * dt / 1000000.0;
  }

The selected sampling frequency for all sensors was :math:`f_s=50Hz` meaning
that every :math:`20ms` the integrated angle from the gyroscope is corrected
with the angle given by the Accelerometer. :numref:`angle_correction_acc` shows
how the correction is performed.

.. _angle_correction_acc:
.. code-block:: c
  :caption: Integrated gyroscope angle correction with accelerometer's angle
            :cite:`Example`

  /** Zumos Accelerometer */
  LSM303 compass;


  /**
   * Read the acceleormeter and adjust the angle
   */
  void sampleAccelerometer() {
    static uint16_t lastUpdate = 0;
    uint16_t m = micros();
    uint16_t dt = m - lastUpdate;
    float gyroAngularSpeed = 0;

    lastUpdate = m;

    compass.read();
    accelerometerAngle = atan2(compass.a.z, -compass.a.x) * 180 / M_PI;

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
    angularPosition = weight * accelerometerAngle + (1 - weight) * angularPosition;
    angularSpeed = (angularPosition - prevAngularPosition) * 1000000.0 / dt;
    prevAngularPosition = angularPosition;

  }

.. note::
  * Note that :code:`angularPosition` is derivated to get :code:`angularSpeed`,
    because both quantities are needed by the state variable model used. For
    more information review the :ref:`State Variable Model <r-state-var-model>`.
  * The sign of the angle has been changed from the one in the original
    balancing example :cite:`Example` to match our reference framework.
  * `src/SegwayLQR/ZumoIMU.ino <https://github.com/pjcuadra/zumosegway/blob/master/src/SegwayLQR/ZumoIMU.ino>`_
    holds the source code that handles the IMU.

.. warning::
  All angles are given in degrees because during implementation it was proved
  that it was easier to catch bugs if the angle was in degrees. One reason for
  this was that degrees are scaled up with respect with radians it was easier
  to catch integer divisions causing the angle to be zero. Furthermore the use
  of degrees is a little more intuitive than radians.


ZumoIMU API
+++++++++++

.. cpp:class:: ZumoIMU

  .. cpp:var:: float accelerometerAngle = 0

    Accelerometer angle

  .. cpp:var:: L3G gyro

    Zumo's Gyro

  .. cpp:var:: LSM303 compass

    Zumo'ss Accelerometer

  .. cpp:var:: float gyroOffsetY

    Gyro's bias

  .. cpp:var:: float prevAngularPosition = 0

    Previous Angular position


  .. cpp:function:: void setupIMU()

    Setup the Gyro and Accelerometer

  .. cpp:function:: void sampleGyro()

    Reads the Gyro changing rate and integrate it adding it to the angle

  .. cpp:function:: void sampleAccelerometer()

    Read the accelerometer and adjust the angle

  .. cpp:function:: void calibrateGyro()

    Calibrate the Gyroscope. Get the bias.


.. only:: html

 .. bibliography:: ../_static/references.bib
  :style: plain
  :filter: docname in docnames
