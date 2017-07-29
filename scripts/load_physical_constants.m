% Sampling constants
T_s = 20e-3;
f_s = 1/T_s;

% Consants (context)
M = 0; % Mass of the cart
m =  0.24200; % Mass of the pendulum (Zumo)
b = 0.1; % Friction coeficient of cart
l = 0.0423; % Height of the zumo
I = (m * l^2) / 3; % Mass momentum of inertia of pendulum 

g =  9.8100; % Gravitational constant
r = 0.019; % Wheele radius

% Motor's constants
Ts2 = 150e-3;
sigma = 1;
omega_n = 3.9/(sigma * Ts2);
gear_ratio = 100.37;
count2cycle = 1/(12* gear_ratio);
motor_in_max = 400; % According to specs
motor_out_max = 2 * pi * f_s * 120 * count2cycle; % Experimentally

% Simplifications
J = I + m*l^2;
m = M + m;
a_1 = 2*sigma*omega_n;
b_1 = omega_n^2;
K = (motor_out_max / motor_in_max) *  b_1;
K_1 = m*l/J;
