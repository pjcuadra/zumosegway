Encoders
--------

The Zumo32U4 includes on-board encoders for closed-loop motor control. In our
application we need to read the angular position and angular speed of the motors
because to the Segway model we used defines these quantities as state variables.
Review the state variable model here.

.. todo::

  Add reference to state variable model.

The optical encoder :cite:`encoder` available in the Zumo32u4 uses the *Sharp GP2S60*
:cite:`Sharp2005`. In the case of the encoders the Zumo Library
:cite:`Library` abstracts all the needed configuration and read/write
operations. The only thing needed for implementation is the interpretation
of the count provided by the encounters.

According to :cite:`encoder` the optical encoder provides 12
:abbr:`CPR (counts per revolution)`. Therefore its count can be interpreted
as shown in :eq:`count2cycles`.

.. math:: cycles = \frac{encodersCount}{12 \times gearRatio}
  :label: count2cycles


According to :cite:`motor50`, :cite:`motor75` and :cite:`motor100` the gear
ratio for the different Zumo modules are shown in
:numref:`gear_ratio_per_model`.

.. _gear_ratio_per_model:
.. table:: Gear Ratio and Count to Degree per Zumo32u4 model

  ========  ============  ==================================
   Model     Gear Ratio    Count to Degree (:math:`^\circ`)
  ========  ============  ==================================
    50:1     51.45         :math:`0.58309^\circ`
    75:1     75.81         :math:`0.39573^\circ`
   100:1     100.37        :math:`0.29889^\circ`
  ========  ============  ==================================

Furthermore we can convert number of cycles to degree with the conversion ratio
:math:`\frac{360^\circ}{1 \times cycle}`. Multiplying :eq:`count2cycles` by this
ratio;

.. math::

  motorAngularPosition = encodersCount \times countToDegrees

With;

.. math::

  countToDegrees = \frac{360}{12 \times gearRatio}

.. note::

  Since the Zumo32u4 has one encoder per motor we decided to estimate the actual
  motor:`motorAngularPosition` as the average of both motors' angular position.

:numref:`encoders_code` shows the implementation of the explained above. Note
that the :math:`motorAngularSpeed` is obtained by derivating
:math:`motorAngularPosition` both are needed by our state variable model. Review
our state variable model [here].

.. todo::

  Add state variable model reference here

.. _encoders_code:
.. code-block:: c
  :caption: Encoders Code

  /** Zumo 100:1 motor gear ratio */
  const float gearRatio = 100.37;
  /** Encoder count to cycle convertion constant */
  const float countToDegrees = 360 / (float)(12.0 * gearRatio);

  /** Zumo encoders */
  Zumo32U4Encoders encoders;

  /**
  * Clear the counters of the encoder
  */
  void clearEncoders() {
    encoders.getCountsAndResetLeft();
    encoders.getCountsAndResetRight();
  }

  /**
  * Sample the encoders
  */
  void sampleEncoders() {
    static float prevPosition = 0;
    static uint16_t lastUpdate = 0;
    static float leftPosition = 0;
    static float rightPosition = 0;
    uint16_t m = micros();
    uint16_t dt = m - lastUpdate;
    lastUpdate = m;

    leftPosition += (float)encoders.getCountsAndResetLeft() * countToDegrees;
    rightPosition += (float)encoders.getCountsAndResetRight() * countToDegrees;
    float motorAngularPosition = -(leftPosition + rightPosition) / 2.0;

    motorAngularSpeed = (motorAngularPosition - prevPosition) * 1000000.0 / dt;
    prevPosition = motorAngularPosition;
  }

.. note::

  * :code:`encoders.getCountsAndResetLeft()` and
    :code:`encoders.getCountsAndResetRight()` get the actual count of the
    respective motor and clear its counter.
  * :math:`motorAngularPosition` is the average of both speeds multiplied by
    :math:`-1` to match our reference frame.

.. only:: html

 .. bibliography:: ../_static/references.bib
  :style: plain
  :filter: docname in docnames
