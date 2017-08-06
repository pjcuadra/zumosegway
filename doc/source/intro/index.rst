Introduction
============

This documentation features the implementation of the Segway behavior in a
Zumo32u4 board. The process started with the modeling of the system according
to the capabilities of the Zumo32u4. Therefore, a :ref:`Zumo32u4 <r-zumo>`
chapter was written with the relevant about the Zumo's IMU, Zumo's encoders
and Zumo's motos.

Secondly a model from the literature was selected. The model in
:cite:`mathworks_model` was firstly analyzed. The developed model in
:cite:`mathworks_model` was develop upon simplifying and considering it as
a cart with an inverted pendulum installed on top. It defined the input as the
force applied to the cart. To use this model the force had to be written in
terms of the torque applied to the pendulum.

The model in :cite:`main_paper` proved to be better applicable for
implementation on a Zumo32u4 since it was in already in terms of torque applied
to the pendulum system and the physical model upon the state variable model
was based could be easily mapped to the Zumo by simplifying it.

None of the model in :cite:`mathworks_model` and :cite:`main_paper` considered
a caterpillar driven system. Nevertheless, :cite:`main_paper` considers
explicitly the inertia moment of the wheels which can be replaced by the inertia
moment of the caterpillar system.

:ref:`Segway Model <r-model>` chapter presents how the model of
:cite:`main_paper` was adapted to the Zumo32u4 considering also actuator's
scaling factors.

Later, in :ref:`LQR Controller Design <r-lqr_desgin>` chapter the design of the
control law using LQR method is presented. The design was done based on the
design done in :cite:`mathworks_model` and :cite:`main_paper`.

After the designing the control law the
:ref:`Controller Implementation <r-controller_imp>` chapter explains how the
controller was implemented.

As expected :cite:`mathworks_model` nor :cite:`main_paper` present a method to
obtain accurate friction coefficients, what makes the model also inaccurate. But
the inaccuracy of the model lead to unstable controlled system. Which showed the
need of add a scale factor in the controlling law.

.. only:: latex

  The online version of this documentation can be seen at
  http://zumosegway.readthedocs.io/en/latest/index.html and the source code
  documented here is hosted as an opensource project on github at
  https://github.com/pjcuadra/zumosegway.
