.. _r-zumo:

Zumo32u4
========

The Zumo32U4 is a hardware development platform includes a built-in
Arduino-compatible ATmega32U4 microcontroller, an LCD, encoders for closed-loop
motor control, and proximity sensors for obstacle detection. It’s
high-performance motors and integrated sensors make it versatile enough to serve
as a general-purpose small robot. :cite:`Pololu2017`. :numref:`zumo32u4_image`
shows the Zumo32u4 Robot.

.. _zumo32u4_image:
.. figure:: https://a.pololu-files.com/picture/0J6721.1200.jpg?f23bb5e39014c5721350a43b8c0e8fe4
  :width: 400px
  :align: center

  Zumo32u4 Robot :cite:`Pololu2017`

For the Segway implementation the Zumo32u4's blades, line sensor array and top
most IR Proximity sensors were removed because the Zumo32u4 balances on the edge
where the blade is (top edge in :numref:`zumo32u4_image`). The opposite edge
can't be used because the battery (not visible in the :numref:`zumo32u4_image`)
holder completely sits on the floor.

At the time of writing this documentation there are three different Zumo32u4
robots;

* Zumo32u4 robot with 50:1 HP motors
* Zumo32u4 robot with 75:1 HP motors
* Zumo32u4 robot with 100:1 HP motors

The main difference between the available configurations is the gear ration of
the motors. For this project the Zumo32u4 robot with 100:1 HP motors was used.

In the following subsections the relevant Zumo32u4 components will be described
in more detail.

.. toctree::
   :maxdepth: 2

   imu.rst
   motors.rst
   encoders.rst

.. only:: html

 .. bibliography:: ../_static/references.bib
  :style: plain
  :filter: docname in docnames
