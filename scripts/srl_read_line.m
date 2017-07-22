function line = srl_read_line(s1)

  line = "";
  
  try
    c = srl_read(s1, 1);
    while (c != 10)    
      line = cstrcat(line, char(c));
      c = srl_read(s1, 1);
      
      k = kbhit (1);
      
      if (k == 'x')
        break;
      endif
      
    endwhile    
  catch
    return;
  end_try_catch

endfunction