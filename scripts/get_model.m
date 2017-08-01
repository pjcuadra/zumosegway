function [plant, model] = get_model()

  % Load needed packages                      
  %pkg load control
 
  % Get the state variable model
  model = get_ssmodel();
  
    % Get the transfer function
  plant = tf(model);
  plant = plant(2);

end