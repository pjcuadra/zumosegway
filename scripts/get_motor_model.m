function motor_tf = get_motor_model()

  % Load needed packages                      
  pkg load control
  
  # Consants (context)
  load_physical_constants
  
  % Steady state gain compensation
  K = (motor_out_max / motor_in_max); % rad/s convertion
  
  % Standard second order system
  %  From the step response of the motor we get
  %    - It's a critically damped system
  %    - The settling time fo the 2%
  motor_tf = tf([K*omega_n^2],[1 2*sigma*omega_n omega_n^2]);

endfunction