.. _r-lqr_desgin:

LQR Controller Design
=====================

The controller to be implemented is a full-state feedback controller. The
LQR controller was selected. :numref:`full_state_fb_block` shows the block
diagram of the entire system to be implemented.


.. _full_state_fb_block:
.. graphviz::
  :align: center
  :caption: Full-state feedback block diagram

   digraph {
    graph [rankdir=LR, splines=ortho, concentrate=true];
    node [shape=polygon];


    node[group=main];
    System [ label="Segway \n ẋ = Ax(t) + Bu(t)", rank=1];
    i [shape=point];
    x [shape=plaintext, label=""];
    C [ label="C" ];
    y [shape=plaintext, label=""];

    System -> i [ dir="none", label="x(t)" ];
    i -> C;
    C -> y [ label="y(t)"];

    node[group="feedback"];
    Feedback [ label="-K", rank=1 ];
    Feedback -> System [ label="u(t)"];
    Feedback -> i [ dir=back ];

   }

Physical parameters
-------------------

All the parameters needed for the model can be seen in :numref:`constants`

.. _constants:
.. code-block:: matlab
  :caption: Physical constant script *scripts/load_physical_constants.m*
            :cite:`repo`

  % Sampling constants
  T_s = 20e-3;
  f_s = 1/T_s;

  % Constants (context)
  m = 0.24200; % Mass of the zumo
  m_1 = 0;
  m_2 = m;
  L_1 = 0;
  L_2 = 0.062;
  L = L_2/2 + (L_1 + L_2) * m_1/(2*m); % Height of the zumo
  beta_m = 0.01;
  beta_gamma = 0.01;

  g =  9.8100; % Gravitational constant
  R = 0.019; % Wheel radius

  I = m_1*(L_1/2 + L_2)^2 + (m_2*L_2^2)/12; % Inertial momentum

  m_w = 0.004; % Mass of the wheel
  m_c = 0.009; % Mass of the caterpillar band

  I_w_i = m_w*R^2; % Inertia momentum of wheels
  I_c = m_c*R^2; % Inertia momentum of Caterpillar band
  I_w = 2*I_w_i + I_c; % Inertia momentum of Caterpillar system

  % Motor's constants
  motor_stall_torque = 0.211846554999999; % According to specs 30 oz-in
  pulse2torque = motor_stall_torque/400;


.. note::
  For :math:`\beta_m` and :math:`\beta_\gamma` are set to a dummy value as in
  :cite:`main_paper`.

Model
-----

To setup the model the script in :numref:`get_ssmodel_m` was used.

.. _get_ssmodel_m:
.. code-block:: matlab
  :caption: Get State Variable Model script *scripts/get_ssmodel.m* :cite:`repo`

  function model = get_ssmodel()

    % Consants (context)
    load_physical_constants

    E = [(I_w + (m_w + m)*R^2) m*R*L;
         m*R*L (I + m*L^2)];

    F = [(beta_gamma + beta_m) -beta_m;
         -beta_m beta_m];

    G = [0; -m*g*L];

    H_1 = [1; -1] * pulse2torque;

    states = size(E, 1);

    A = [zeros(states) eye(states);
         zeros(states, 1) -inv(E)*G -inv(E)*F];
    B = [zeros(states, 1);
         -inv(E)*H_1];
    C = [1 0 0 0;
         0 1 0 0;
         0 0 1 0;
         0 0 0 1];
    D = [0; 0; 0; 0];

    model = ss(A, B, C, D);

  end

A second script, shown in :numref:`get_model_m` was also added to get the model
that also obtains the transfer function :math:`H(s) = \frac{\Theta(s)}{S(s)}`.
Where, :math:`S(s)` is the Laplace transform of the :math:`speed_{PWM}`
function. This transfer function was used to further analysis not presented in
this documentation.

.. _get_model_m:
.. code-block:: matlab
  :caption: Get Model script *scripts/get_model.m* :cite:`repo`

  function [plant, model] = get_model()

    % Get the state variable model
    model = get_ssmodel();

    % Get the transfer function
    plant = tf(model);
    plant = plant(2);

  end

Controllability
---------------

Before the actually designing the controller we need to check it's
controllability. The controllability check done can be seen in
:numref:`control_check_m`.

.. _control_check_m:
.. code-block:: matlab
  :caption: Check controllability *scripts/lqr_design.m* :cite:`repo`

  % Check controlability
  co =  ctrb(model);
  if (rank(co) > n_states)
    disp(" -> Error! System isn't controllable");
    return;
  else
    disp(" -> Great! System is controllable");
  end

Observability
-------------

Similarly, the system's observability has to be also verified. This verification
is shown in :numref:`obs_check_m`.

.. _obs_check_m:
.. code-block:: matlab
  :caption: Check Observability *scripts/lqr_design.m* :cite:`repo`

  ob = obsv(model);
  if (rank(ob) > n_states)
    disp(" -> Error! System isn't observable");
    return;
  else
    disp(" -> Great! System is observable");
  end


Control Law
-----------

To obtain the control law :math:`K` the script in :numref:`lqr_des_m`.

.. _lqr_des_m:
.. code-block:: matlab
  :caption: Control Law Calculation *scripts/lqr_design.m* :cite:`repo`

  Q = eye(size(model.a,1));

  R = 1

  [K, X, P] = lqr(model, Q, R);

  K_s = K*pi/180;

.. note::
  * As in :cite:`main_paper` equally weighted states and outputs were used.
    Therefore, :math:`Q = I` and :math:`R = 1`. Where, :math:`I` is an identity
    matrix with the size of :math:`A`.
  * A scaled control law :math:`K_s` is also calculated. The scale factor is
    needed because the angles measured by accelerometer/gyro and encoders
    was done in degrees.

The obtained control law is shown in :eq:`res_control_law`. And the scaled
version in :eq:`res_s_control_law`

.. math:: K = \begin{bmatrix} 1 & 483.6133 & 10.0038 & 20.3053 \end{bmatrix}
  :label: res_control_law

.. math:: K_s = \begin{bmatrix} 0.0175 & 8.4406 & 0.1746 & 0.3544 \end{bmatrix}
  :label: res_s_control_law


Controller Simulation
---------------------

After designing the control law the controller is simulated as shown in
:numref:`sim_lqr_des_m`.

.. _sim_lqr_des_m:
.. code-block:: matlab
  :caption: Controller simulation excerpt *scripts/lqr_design.m* :cite:`repo`

  Ac = model.a - model.b*K;
  sys_cl = ss(Ac, model.b, model.c, model.d);
  figure(1);
  clf(1)
  impulse(sys_cl);

The simulation results can be seen in :numref:`sim_res`. Since the
:math:`\frac{d\varphi}{dt}` and :math:`\frac{d\theta}{dt}` are faster variables
a zoomed result simulation result can also be seen in :numref:`zoomed_sim_res`.

.. _sim_res:
.. figure:: ../_static/sim_results.png
  :align: center

  LQR Design Simulation Results


.. _zoomed_sim_res:
.. figure:: ../_static/zoomed_sim_results.png
  :align: center

  LQR Design Zoomed Simulation Results

Further Features
----------------

Arduino Pretty Printing
***********************

For rapid deployment and testing the
`scripts/lqr_design.m <https://github.com/pjcuadra/zumosegway/blob/master/scripts/lqr_design.m>`_
also prints the scaled control law in the Arduino language. An example output
can be seen in :numref:`k_cpp_print_out_m` and the script excerpt that implement
this functionality in :numref:`k_cpp_print_m`.

.. _k_cpp_print_m:
.. code-block:: matlab
  :caption: Arduino Code control law generator *scripts/lqr_design.m*
            :cite:`repo`

  disp("Control Law")
  K_string = strcat("const float K[", num2str(size(model.a,1)), "] = {");
  for k = 1:size(K, 2)
    if ~(k == 1)
      K_string = strcat(K_string, ", ");
    end
    K_string = strcat(K_string, num2str(K_s(k)));
  end
  K_string = strcat(K_string, "};");
  disp("K")
  disp(K_string)

.. _k_cpp_print_out_m:
.. code-block:: matlabsession
  :caption: Arduino Code control law generator output

  Control Law
  K
  const float K[4] = {0.017453, 8.4406, 0.1746, 0.35439}

Closed-Loop Poles
*****************

`scripts/lqr_design.m <https://github.com/pjcuadra/zumosegway/blob/master/scripts/lqr_design.m>`_
also print out the closed-loop poles. An example output can be seen in
:numref:`poles_out_ex_m`.

.. _poles_out_ex_m:
.. code-block:: matlabsession
  :caption: Closed-loop printing output example

  P =

   1.0e+03 *

   -1.0872
   -0.0000
   -0.0083
   -0.0178


Full-compensator design
***********************

In `scripts/lqr_design.m <https://github.com/pjcuadra/zumosegway/blob/master/scripts/lqr_design.m>`_
the full compensator design flow is implemented but since it's not being
implements it's was left out of this documentation.

.. only:: html

 .. bibliography:: ../_static/references.bib
  :style: plain
  :filter: docname in docnames
