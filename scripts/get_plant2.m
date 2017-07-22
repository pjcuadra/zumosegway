function [plant, servo] = get_plant2()

  # Consants (context)
  l = 0.0423; # Height of the zumo
  m =  0.24200; # Mass of the zumo
  g =  9.8100; # Gravitational constant
  b = 0;
  J = m * l^2 /3;
  q = m*J;

  % Load needed packages                      
  pkg load control
  
  % Create the system for the plant
  plant = tf([m*l/q 0], [1 b*(J+m*l^2)/q -m^2*g*l/q -b*m*g*l/q]);

endfunction