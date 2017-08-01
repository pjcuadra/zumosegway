function model = get_ssmodel()

  % Consants (context)
  load_physical_constants
  
  E = [(I_ws + (m_w + m)*R^2) m*R*l;
       m*R*l (I + m*l^2)];
       
  F = [(beta_gamma + beta_m) -beta_m;
       -beta_m beta_m];
       
  G = [0; -m*g*l];
  
  H = [1; -1];
  
  states = size(E, 1);
  
  A = [zeros(states) eye(states);
       zeros(states, 1) -inv(E)*G -inv(E)*F];
  B = [zeros(states, 1);
       -inv(E)*H];
  B= B * pulse2torque; % Convert input to be pulse instead of torque
  C = [1 0 0 0;
       0 1 0 0;
       0 0 1 0;
       0 0 0 1];
  D = [0; 0; 0; 0];
  
  model = ss(A, B, C, D);
  
end            