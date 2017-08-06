.. _r-model:

Segway Model
============

The model of the balancing robot proposed in :cite:`main_paper` is derived from
the physical description of :numref:`model_image`.

.. _model_image:
.. figure:: ../_static/model_image.png
  :align: center
  :width: 200px

  Segway's relevant parameters :cite:`main_paper`


Where;

* :math:`x` is the horizontal position of the center of the wheel.
* :math:`\varphi` is the clockwise rotation angle of the wheel from the
  horizontal axis
* :math:`\theta` is the clockwise rotation angle of the Zumo32u4 from the
  horizontal axis
* :math:`m` is the mass of the entire robot
* :math:`m_w` is the mass of the wheel
* :math:`R` radius of the wheel
* :math:`L` length between the center of the wheel and the
  :abbr:`COM (center of mass)`
* :math:`\tau_0` is the applied torque
* :math:`I` inertia of the body part
* :math:`I_w` inertia of the wheel

System's dynamics equations
---------------------------

The obtained differential equations of the system in :cite:`main_paper` are;

.. math::
  :label: system_dyn

  E
  \begin{bmatrix}
    \ddot{\varphi} \\
    \ddot{\theta}
  \end{bmatrix} \
  \
  + F
  \begin{bmatrix}
    \dot{\varphi} \\
    \dot{\theta}
  \end{bmatrix} \
  \
  + G \theta = H\tau_0

With,

.. math::

  E = \begin{bmatrix}
    I_w + (m_w + m)R^2 & mRL \\
    mRL                & I + mL^2
  \end{bmatrix}

.. math::

  F = \begin{bmatrix}
    \beta_\gamma + \beta_m & -\beta_m \\
    -\beta_m               & \beta_m
  \end{bmatrix}

.. math::

  G = \begin{bmatrix}
    0 \\
    -mgL
  \end{bmatrix}

.. math::

  H = \begin{bmatrix}
    1 \\
    -1
  \end{bmatrix}

Model Adaptation
----------------

In :cite:`main_paper` they define :math:`L` as in :eq:`com` with the variables
define as in :numref:`com_fig`.

.. math:: L = \frac{L_2}{2} + \frac{L_1 + L_2}{2}\frac{m_1}{m}
  :label: com

.. _com_fig:
.. figure:: ../_static/com_image.png
  :align: center
  :width: 200px

  Center of Mass calculation :cite:`main_paper`

Similarly :cite:`main_paper` defines the inertia momentum of the robot as in
:eq:`inertial_mom`.

.. math:: I = m_1(\frac{L_1}{2}+L_2)^2 \frac{1}{12}m_2L_2^2
  :label: inertial_mom

Given the geometry of the Zumo32u4 we consider that :math:`m_1 = 0` and
:math:`L_1 = 0`. Therefore the distance to the :abbr:`COM (Center Of Mass)`
and the inertia momentum can be calculated :math:`L = \frac{L_2}{2}` and
:math:`I = \frac{1}{12}m_2L_2^2`, respectively.

Furthermore, the model in :cite:`main_paper` consider a normal wheel. In our
Zumo32u4 we have a caterpillar system. :eq:`cat_sys_inertia` shows how the
inertia moment of the caterpillar system was calculated.

.. math:: I_w = I_{w_1} + I_c + I_{w_2}
  :label: cat_sys_inertia

Where the :math:`I_{w_1}` and :math:`I_{w_2}` are the inertia moment of the
both wheels and :math:`I_c` is the inertia moment of the caterpillar band. In
our case both wheels are equal and can be calculated as in :eq:`wheel_inertia`

.. math:: I_{w_i} = m_wR^2
  :label: wheel_inertia

Additionally the inertia of the caterpillar band can be calculated as shown in
:eq:`cat_inertia`. Where :math:`m_c` is the mass of the caterpillar band.

.. math:: I_c = m_cR^2
  :label: cat_inertia

Finally the inertia moment of the entire caterpillar system can be calculated
as in :eq:`simplify_inertia`.

.. math:: I_w = (2\cdot m_w + m_c)R^2
  :label: simplify_inertia


Input Adaptation
----------------

The model in :cite:`main_paper` defines the input to be the torque
:math:`\tau_0`. Since the actual input to our system is the PWM applied to the
motors we can use the equation defined in the subsection Motors of
the chapter Zumo32u4, shown in :eq:`torque_relation`.

.. math:: \tau_0 = \frac{\tau_s}{400} \times speed_{PWM}
  :label: torque_relation

Merging :eq:`torque_relation` and :eq:`system_dyn` we obtain;

.. math::

  E
  \begin{bmatrix}
    \ddot{\varphi} \\
    \ddot{\theta}
  \end{bmatrix} \
  \
  + F
  \begin{bmatrix}
    \dot{\varphi} \\
    \dot{\theta}
  \end{bmatrix} \
  \
  + G \theta = H_1speed_{PWM}

With;

.. math::
  H_1 = H * \frac{\tau_s}{400}


.. _r-state-var-model:

State Variable Model
--------------------

Finally the state variable model of the system can be calculated as shown in
:eq:`gen_state_var`.

.. math::
  :label: gen_state_var

  \dot{x} = Ax + Bu

  y = Cx + D

With the state variable vector;

.. math::
  x = \begin{bmatrix}
    \varphi \\
    \theta \\
    \dot{\varphi} \\
    \dot{\theta}
  \end{bmatrix}

And the constant matrices;

.. math::
  A = \left[
  \begin{array}{c|c|cc}
    \begin{matrix} 0 \\ 0 \end{matrix} & \begin{matrix} 0 \\ 0 \end{matrix}  & \begin{matrix} 1 & 0 \\ 0 & 1 \end{matrix} \\
    \hline
    \begin{matrix} 0 \\ 0 \end{matrix} & -E^{-1}G & -E^{-1}F
  \end{array}
  \right]


.. math::
  B = \left[
  \begin{array}{c}
    \begin{matrix} 0 \\ 0 \end{matrix} \\
    \hline
    -E^{-1}H
  \end{array}
  \right]

.. math::

  C = \begin{bmatrix}
    1 & 0 & 0 & 0 \\
    0 & 1 & 0 & 0 \\
    0 & 0 & 1 & 0 \\
    0 & 0 & 0 & 1
  \end{bmatrix}

.. math::

  D =  \begin{bmatrix}
    0 \\
    0 \\
    0 \\
    0
  \end{bmatrix}



.. only:: html

 .. bibliography:: ../_static/references.bib
  :style: plain
  :filter: docname in docnames
