function [plant, model] = get_plant()

  % Load needed packages                      
  pkg load control
 
  % Get the state variable model
  model = get_ssmodel();
  
    % Get the transfer function
  plant = get_tf();

endfunction