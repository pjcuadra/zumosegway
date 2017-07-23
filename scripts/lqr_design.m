clear;
clc;

pkg load control
pkg load symbolic

p0 = -i;
p1 = i;

% Get the model
[model] = get_statemodel()

% Check controlability
co =  ctrb(model);
if (det(co) == 0)
  disp(" -> Error! System isn't controllable");
  return;
else
  disp(" -> Great! System is controllable");
endif

ob = obsv(model);
if (det(ob) == 0)
  disp(" -> Error! System isn't observable");
  return;
else
  disp(" -> Great! System is observable");
endif

% Get the control law
syms s;
q_k = (s - p0)*(s - p1);
K = double(expand([0 1]*inv(co)*subs(q_k, model.a)))

% Get L
syms s;
q_l = (s - p0)*(s - p1);
L = double(subs(q_k, model.a)*inv(ob)*[0; 1])


