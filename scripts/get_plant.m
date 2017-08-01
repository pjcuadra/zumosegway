function [plant, model] = get_plant()

  % Load needed packages                      
  pkg load control

  % Get the transfer function
  plant = get_tf();
  
  % Get the state variable model
  model = get_ssmodel();

endfunction