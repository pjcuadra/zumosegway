function model = get_ssmodel()

  % Load needed packages                      
  pkg load control
  
  # Consants (context)
  load_physical_constants
           
  A = [0 1; g/l 0];
  B = [0; -l];
  C = [1 0; 0 1];
  D = [0; 0];
  
  model = ss(A, B, C, D);
  
endfunction                 