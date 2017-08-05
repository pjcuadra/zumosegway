.. _motors_title:

Motors
------

The Zumo32U4 includes a *DRV8837* :cite:`motordriver` as a dual motor driver.
The *DRV8837* handles the current requirements for motors. It also provides
rotational direction control of the motors.

The Zumo Library :cite:`Library` provides the following API to control the
motors' speed.

.. cpp:class:: Zumo32U4Motors

   Controls motor speed and direction on the Zumo 32U4.

  .. cpp:function:: void setSpeeds(int16_t leftSpeed, int16_t rightSpeed)

    Sets the speeds for both motors.

    .. cpp:var:: int16_t leftSpeed

      A number from -400 to 400 representing the speed and direction of the
      left motor. Values of -400 or less result in full speed reverse, and
      values of 400 or more result in full speed forward.

    .. cpp:var:: int16_t rightSpeed

      A number from -400 to 400 representing the speed and direction of the
      right motor. Values of -400 or less result in full speed reverse, and
      values of 400 or more result in full speed forward.


:cpp:func:`Zumo32U4Motors::setSpeeds()` enables us to control both motors. For
instance a :math:`400` value in :cpp:var:`leftSpeed` will set the a 100% duty
cycle with level :math:`6V` PWM between the left motor's *DRV8837*'s outputs
(OUT1 and OUT2). Which results in applying full forward speed to the left motor.
A value of :math:`-200` will set a 50% duty cycle with level of :math:`-6V` PWM
between the left motor's *DRV8837*'s outputs (OUT1 and OUT2).

The PWM duty cycle can be translated into a percentage of maximum current
applied to the motor :math:`I_{max}`. :numref:`motors_specs`'s shows the value
of :math:`I_{max}` which corresponds to the Stall Current at 6V. Additionally,
the torque of the motor is linearly related to the current is applied to it.
Therefore, the torque of the motors can be calculated by;

.. math:: \tau_0 = \frac{\tau_s}{400} \times speed_{PWM}
  :label: torque_relation

Where :math:`\tau_s` is the stall torque of the motor at 6V. In our case we used
:math:`\tau_t = 0.211846 Nm`.

.. _motors_specs:
.. table:: Motors' Torque related information per Zumo32u4 model
           :cite:`Pololu2017a`

  ========  ================================  ===================================  ================================
   Model     Stall Torque @ 6V (:math:`Nm`)    Free-run Speed @ 6V (:math:`RPM`)    Stall Current @ 6V (:math:`A`)
  ========  ================================  ===================================  ================================
    75:1     :math:`0.105923`                  :math:`625`                          :math:`1.6`
    50:1     :math:`0.155354`                  :math:`400`                          :math:`1.6`
   100:1     :math:`0.211846`                  :math:`320`                          :math:`1.6`
  ========  ================================  ===================================  ================================

.. only:: html

 .. bibliography:: ../_static/references.bib
  :style: plain
  :filter: docname in docnames
