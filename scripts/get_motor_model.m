function motor_tf = get_plant()

  % Load needed packages                      
  pkg load control
  
  # Consants (context)
  load_physical_constants
  
  K = 2 * pi * out_max / in_max; % rad/s convertion
  
  motor_tf = tf([K*omega_n^2],[1 2*sigma*omega_n omega_n^2]);


endfunction