function plant = get_tf()

  % Load needed packages                      
  %pkg load control
  
  # Consants (context)
  load_physical_constants
  
%  tf_pendulum = tf([K_1*r 0], [1 0 -(K_1 * g)]);
%  tf_motor = tf([K], [1 a_1 b_1]);
    
  % Create the system for the plant
%  plant = tf_pendulum*tf_motor;
  plant = 0;

endfunction