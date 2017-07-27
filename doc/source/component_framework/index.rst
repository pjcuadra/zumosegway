Component Framework
===================

Component
---------
The *Component* can be realized by the fundamental elements from which model of
system-of-interest can be built.

To the build the model using components we took the following considerations;
* Components are implemented as a C++ class.
* Replaceable and autonomous within its environment.
* Components are characterized by Signals and Ports.
* Accessible by other components via Ports.

Signal
++++++

The Components can be interact with its environment or with other components by
means of *Signal*.

For eg.
..code::

// Signals
/** Angle of the Segway with respect to the perpendicular to the floor */
Signal angle;
/** Angular speed of the Segway */
Signal angular_speed;
/** Linear speed of the Segway */
Signal speed;
/** Corrected Angle because of the deviation of the center of gravity */
Signal corrected_angle;
/** Negative of the angle of the center of gravity */
Signal center_angle;

Port
++++

The *Port* provides a distinct interaction point for communication
* Between component and its environment
* Between that component and its internal parts

For eg.
..code::

/** Angle output port */
Port angle_out;
/** Angular speed output port */
Port angular_speed_out;

API for Component Framework
---------------------------

We have recursively built components such as blablabla..........(Pedro)
