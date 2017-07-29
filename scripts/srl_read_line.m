function line = srl_read_line(s1)

  line = "";
   
  % Get start time
  t = clock();
    
  % Initialize with dummy value
  c = 0;
  
  while(1)    
    [c, read_char] = srl_read(s1, 1);
    
    % If no character was read retry
    if (read_char == 0)
      error("Timeout reading character");
      continue;
    endif
    
    % Throw error if over 2 secons are waited
    if (etime(clock(), t) >= 1)
      error("Timeout reading line");
      return;
    endif
      
    
    % End while line break
    if (c == 10) 
      break;
    endif
    
    line = cstrcat(line, char(c));
          
  endwhile    

endfunction