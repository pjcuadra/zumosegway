function [plant, servo] = get_plant()

  # Consants (context)
  l = 0.0423; # Height of the zumo
  m =  0.24200; # Mass of the zumo
  g =  9.8100; # Gravitational constant
  r_w = 0.019; # Wheel radius
  K_s = pi * r_w / 60 # Proportionality constant between rotational and linear 
                     # speed
  K_s = 0.012;

  % Load needed packages                      
  pkg load control
  
  servo = tf([K_s], [l]);

  % Create the system for the plant
  plant = servo*tf([-1 0], [l 0 -g]);

endfunction