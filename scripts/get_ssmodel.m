function model = get_ssmodel()

  % Load needed packages                      
  pkg load control
  
  # Consants (context)
  load_physical_constants
  
  
  % State vector defined as
  % x = [x dx/dt theta dtheta/dt omega domega/dt]'
  A = [0 1      0       0 0       0;
       0 -b/m   0       0 b*r/m   r;
       0 0      0       1 0       0;
       0 -l*b/J m*g*l/J 0 b*r*l/J m*l*r/J;
       0 0      0       0 0       1;
       0 0      0       0 -b_1    -a_1;    
       ];
  B = [0;
       0;
       0;
       0;
       0;
       K];
  C = [0 0 1 0 0 0;
       0 0 0 1 0 0; 
       0 0 0 0 1 0];
  D = [0; 0; 0];
  
  model = ss(A, B, C, D);
  
endfunction                 