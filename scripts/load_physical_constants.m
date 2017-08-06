% Sampling constants
T_s = 20e-3;
f_s = 1/T_s;

% Constants (context)
m = 0.24200; % Mass of the zumo
m_1 = 0;
m_2 = m;
L_1 = 0;
L_2 = 0.062;
L = L_2/2 + (L_1 + L_2) * m_1/(2*m); % Height of the zumo
beta_m = 0.01;
beta_gamma = 0.01;

g =  9.8100; % Gravitational constant
R = 0.019; % Wheel radius

I = m_1*(L_1/2 + L_2)^2 + (m_2*L_2^2)/12; % Inertial momentum

m_w = 0.004; % Mass of the wheel
m_c = 0.009; % Mass of the caterpillar band

I_w_i = m_w*R^2; % Inertia momentum of wheels
I_c = m_c*R^2; % Inertia momentum of Caterpillar band
I_w = 2*I_w_i + I_c; % Inertia momentum of Caterpillar system

% Motor's constants
motor_stall_torque = 0.211846554999999; % According to specs 30 oz-in 
pulse2torque = motor_stall_torque/400;
