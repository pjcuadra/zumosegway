function model = get_ssmodel()

  % Consants (context)
  load_physical_constants
  
  E = [(I_w + (m_w + m)*R^2) m*R*L;
       m*R*L (I + m*L^2)];
       
  F = [(beta_gamma + beta_m) -beta_m;
       -beta_m beta_m];
       
  G = [0; -m*g*L];
  
  H_1 = [1; -1] * pulse2torque;
  
  states = size(E, 1);
  
  A = [zeros(states) eye(states);
       zeros(states, 1) -inv(E)*G -inv(E)*F];
  B = [zeros(states, 1);
       -inv(E)*H_1];
  C = [1 0 0 0;
       0 1 0 0;
       0 0 1 0;
       0 0 0 1];
  D = [0; 0; 0; 0];
  
  model = ss(A, B, C, D);
  
end            