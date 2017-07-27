% Clear everything
clear;
clc;

% Load needed packages                      
pkg load control
  
% Get the transfer function
motor_tf = get_motor_model()

% Experimentally obtained
P = 20; % Selected to have a smooth speed control / avoid reaching saturation
I = 200; % Makes the response faster
D = 0; % No need for this parameter

% PID
pid_c = pid(P, I ,D)

% Check the step response
step(feedback(pid_c*motor_tf));