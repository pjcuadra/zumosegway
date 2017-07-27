% Sampling constants
T_s = 20e-3;
f_s = 1/T_s;

% Consants (context)
l = 0.0423; # Height of the zumo
m =  0.24200; # Mass of the zumo
g =  9.8100; # Gravitational constant
r_w = 0.019; % Wheele radius

% Motor's constants
Ts2 = 150e-3;
sigma = 1;
omega_n = 3.9/(sigma * Ts2);
gear_ratio = 100.37;
count2cycle = 1/(12* gear_ratio);
motor_in_max = 400; % According to specs
motor_out_max = 2 * pi * f_s * 120 * count2cycle; % Experimentally

