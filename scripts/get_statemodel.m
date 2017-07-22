function [model] = get_statemodel()
  # Consants (context)
  l = 0.0423; # Height of the zumo
  m =  0.24200; # Mass of the zumo
  g =  9.8100; # Gravitational constant
  r_w = 0.019; # Wheel radius
  K_s = 2 * pi * r_w / 60; # Proportionality constant between rotational and linear 
                     # speed
           
  A = [0 1; g/l 0];
  B = [0; -K_s/l];
  C = [1 0];
  D = [0];
  
  model = ss(A, B, C, D);
  
endfunction                 