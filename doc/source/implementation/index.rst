Controller Implementation
=========================

The implementation of the controller system was done using Arduino IDE.
The functionality was separated into files within the Arduino IDE project.
*ZumoIMU* and *ZumoEncoders* files where already explained in the sections
:ref:`Inertial Management Unit <r-imu>` and :ref:`Encoders <r-encoders>`.

SegwayLQR
---------

SegwayLQR API
*************

Global Constants
++++++++++++++++

.. cpp:var::  const uint8_t samplingPeriodMS = 20

  Sampling Period in ms

.. cpp:var::  const float samplingPeriod = samplingPeriodMS / 1000.0;

  Sampling Period in s

.. cpp:var::  const float samplingFrequency = 1/samplingPeriod

  Sampling frequency

.. cpp:var::  const uint8_t statesNumber = 4

  Number states


Global Variables
++++++++++++++++

.. cpp:var::  float angularPositionLP = 0

  Low pass filter angular Position

.. cpp:var::  float angularPosition = 0

  Zumo's angular position

.. cpp:var::  float correctedAngularPosition = 0

  Corrected angular position

.. cpp:var::  float angularSpeed = 0

  Zumo's angular speed

.. cpp:var::  float motorAngularPosition = 0

  Motor's angular position

.. cpp:var::  float motorAngularSpeed = 0

  Motor's angular speed

.. cpp:var:: int32_t speed

  PWM signal applied to the motor's driver 400 is 100% cycle and -400 is 100%
  but inverse direction


.. cpp:var:: Zumo32U4ButtonA buttonA

  A button of the zumo board

.. cpp:var:: Zumo32U4Motors motors

  Zumo robot's motors


Functions
+++++++++

.. cpp:class:: SegwayLQR

  .. cpp:function:: void setup()

    Segway's setup function.

  .. cpp:function:: void loop()

    Segway's loop function.

  .. cpp:function:: void setActuators()

    Set the values to the actuators.

SegwayLQR Details
*****************

`src/SegwayLQR/SegwayLQR.ino <https://github.com/pjcuadra/zumosegway/blob/master/src/SegwayLQR/SegwayLQR.ino>`_
features the main loop and setup functions of the Arduino project.
:numref:`setup_function` shows the implementation of the setup function.
Firstly, it setups the IMU by calling :cpp:func:`ZumoIMU::setupIMU()`.
Then, calibrates the IMU's gyro by calling
:cpp:func:`ZumoIMU::calibrateGyro()`. After calibrating it starts a loop of
sampling the gyro as frequently as possible and the accelerometer every
:cpp:var:`samplingPeriod`. When the buttonA is pressed the loop is exited and
before starts executing :cpp:func:`SegwayLQR::loop` the encoders counters
are cleared by calling :cpp:func:`ZumoEncoders::clearEncoders()`.

.. _setup_function:
.. code-block:: cpp
  :caption: Setup function

  /**
   * Setup Function
   */
  void setup() {
    Wire.begin();

    Serial.begin(115200);

    // Setup the IMU
    setupIMU();

    // Calibrate the IMU (obtain the offset)
    calibrateGyro();

    // Display the angle until the user presses A.
    while (!buttonA.getSingleDebouncedRelease()) {
      // Update the angle using the gyro as often as possible.
      sampleGyro();

      // Sample accelerometer every sampling period
      static uint8_t lastCorrectionTime = 0;
      uint8_t m = millis();
      if ((uint8_t)(m - lastCorrectionTime) >= samplingPeriodMS)
      {
        lastCorrectionTime = m;
        sampleAccelerometer();
      }
    }
    delay(500);
    clearEncoders();
  }


:numref:`loop_function` shows the loop function's code. Basically does the
same as in the loop in :cpp:func:`SegwayLQR::setup` but every
:cpp:var:`samplingPeriod` it;

1. :cpp:func:`ZumoIMU::sampleAccelerometer()` to obtain the corrected
   estimation of the Zumo's angle and angular speed, as explained in
   :ref:`Inertial Management Unit <r-imu>`.
2. :cpp:func:`ZumoEncoders::sampleEncoders()` to obtain encoders position
   and speed, as explained in :ref:`Encoders <r-encoders>`.
3. :cpp:func:`SegwayLQR::setActuators` calculates the new speed to be set
   based on the current state variables' state and the LQR designed control law.

.. _loop_function:
.. code-block:: cpp
  :caption: Loop function

  /**
   * Main loop Function
   */
  void loop() {
    // Update the angle using the gyro as often as possible.
    sampleGyro();

    // Every 20 ms (50 Hz), correct the angle using the
    // accelerometer, print it, and set the motor speeds.
    static byte lastCorrectionTime = 0;
    byte m = millis();
    if ((byte)(m - lastCorrectionTime) >= 20)
    {
      lastCorrectionTime = m;
      sampleAccelerometer();
      sampleEncoders();
      setActuators();
    }
  }

:numref:`act_function` shows the :cpp:func:`SegwayLQR::setActuators`
function's code. As a security measure when the angle is greater that
:math:`45^\circ` the speed is set to zero. Furthermore, the angle is corrected
by the deviation of the :abbr:`COM (Center Of Mass)` from the actual horizontal
center of the Zumo32u4. Finally the :cpp:func:`LQR::lqr` is called to apply
the control law and generate the input of the system.

.. _act_function:
.. code-block:: cpp
  :caption: Set actuators function

  /**
   * Control the actuators
   */
  void setActuators() {
    const float targetAngle = 1.45;

    if (abs(angularPosition) > 45) {
      // If the robot is tilted more than 45 degrees, it is
      // probably going to fall over.  Stop the motors to prevent
      // it from running away.
      speed = 0;
    } else {
      correctedAngularPosition = angularPosition - targetAngle;
      lqr();
      speed = constrain(speed, -400, 400);
    }

    motors.setSpeeds(speed, speed);
  }

LQR
---

LQR API
*******

.. cpp:class:: LQR

  .. cpp:var:: const float K[statesNumber] = {0.55192, 8.9867, 0.194, 0.39237}

    Control Law

  .. cpp:var:: const float scaleConst = 2.5

    Correction factor gain factor

  .. cpp:function:: void lqr()

    Apply LQR control law


LQR Details
***********

:numref:`lqr_function` shows how the :cpp:func:`LQR::lqr` is implemented.

.. _lqr_function:
.. code-block:: cpp
  :caption: Apply LQR designed control law

  /**
   * LQR control law
   */
  void lqr() {
    speed = 0;

    speed -= motorAngularPosition * K[0];
    speed -= correctedAngularPosition * K[1];
    speed -= motorAngularSpeed * K[2];
    speed -= angularSpeed * K[3];

    speed = speed*scaleConst;

  }

.. note::
  * The K values are multiplied by :math:`-1` in according to
    :numref:`full_state_fb_block`.
  * An additional scale factor, :math:`scaleConst = 2.5`, is introduce to
    compensate;

    * Possible deviation of the actual Stall Torque with load.
    * Bad estimation of the :math:`\beta_m` and :math:`\beta_gamma` values.



.. only:: html

 .. bibliography:: ../_static/references.bib
  :style: plain
  :filter: docname in docnames
