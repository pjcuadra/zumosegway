clear;
clc;

% Load needed packages
pkg load control

graphics_toolkit("gnuplot") 

% Measurements
oscilations_count = 12.5;
measured_samples = 100;
sampling_period = 20;

% Constants
plots_row = 4;
plots_col = 1;

disp("========================================================================")
disp(" Parameters")
disp("========================================================================")
K0 = 53
T0 = sampling_period * (measured_samples/oscilations_count) / 1000
f0 = 1 / T0

disp("========================================================================")
disp(" Plant")
disp("========================================================================")
[plant, servo] = get_plant()

disp("========================================================================")
disp(" PID Tight")
disp("========================================================================")
K_p = 0.6 * K0
K_i = 2 * K_p * f0
K_d = T0 * K_p  / 8

pid_c = pid(K_p, K_i, K_d);

subplot(plots_row, plots_col, 1);
impulse(feedback(pid_c*plant))
title ("Step Response - PID Tight");

disp("========================================================================")
disp(" PID (Someovershot)")
disp("========================================================================")
K_p = K0 / 3
K_i = K_p * 2 * f0
K_d = K_p * T0 / 3

pid_c = pid(K_p, K_i, K_d);

subplot(plots_row, plots_col, 2);
impulse(feedback(pid_c*plant))
title ("Step Response - PID (someovershoot)");

disp("========================================================================")
disp(" PID (No Overshot)")
disp("========================================================================")
K_p = K0 / 5
K_i = K_p * 2 * f0
K_d = K_p * T0 / 2

pid_c = pid(K_p, K_i, K_d);

subplot(plots_row, plots_col, 3);
impulse(feedback(pid_c*plant))
title ("Step Response - PID (No Overshot)");

disp("========================================================================")
disp(" PID (Manual)")
disp("========================================================================")
K_p = 35
K_i = 15
K_d = 40

pid_c = pid(K_p, K_i, K_d);

subplot(plots_row, plots_col, 4);
impulse(feedback(pid_c*plant))
title ("Step Response - PID (Manual)");

disp("========================================================================")