clear;
clc;

% Needed packages
pkg load instrument-control

% Constants
BUFF_SIZE = 256;
BAUDRATE = 115200;
DEVICE = '/dev/rfcomm0';
REFRESH_RATE = 10;

refresh_count = 0;

% Connect to the serial port
try
  s1 = serial(DEVICE, BAUDRATE)
catch
  disp("Serial port coun't be opened");
  return;
end_try_catch

try
  srl_flush(s1);

  %% Clean the last incompleted line
  srl_read_line(s1)

  y = {};
  t = {};
  count = {};

  while (true)
    line = srl_read_line(s1);
    
    k = kbhit (1);
    
    if (k == 'q')
      break;
    endif

    %% Split the line
    cstr = strsplit(line, ' ');

    %% Get the channel
    fig_num = str2num(cstr{1});

    %% Channel 0 is for info/config
    if (fig_num == 0)
      continue;
    endif

    %% If the size is bigger it's malformed
    if (size(cstr)(2) < 2)
      continue;
    endif

    if (size(t) < fig_num)
      t{fig_num} = [];
    endif

    %% Initialize the array
    if (size(t{fig_num}) == 0)
      y{fig_num} = [];
      t{fig_num} = [];
      count{fig_num} = 0;
    endif

    % if (fig_num != 2)
    %   continue;
    % endif

    count{fig_num}++;
    curr_value_idx = size(t{fig_num})(2) + 1;

    %% If the buffer is full shift all left
    if (size(t{fig_num})(2) == BUFF_SIZE)
      curr_value_idx = BUFF_SIZE;

      t{fig_num}(1:BUFF_SIZE - 1) = t{fig_num}(2:BUFF_SIZE);
      y{fig_num}(1:BUFF_SIZE - 1) = y{fig_num}(2:BUFF_SIZE);
    endif

    t{fig_num}(curr_value_idx) = count{fig_num};

    y{fig_num}(curr_value_idx) = str2num(char(cstr(2)));

    
    
    % Plot every 20 values
    refresh_count++;
    
    if (refresh_count > REFRESH_RATE) 
      refresh_count = 0;
      figure(fig_num);
      plot(t{fig_num}, y{fig_num});
      drawnow();
    endif
    
    
    % pause(1)

  endwhile

catch err
  disp("Error on line: ");
  disp(err.stack.line);
  warning(err.identifier, err.message);
end_try_catch

fclose(s1);
