clear;
clc;

pkg load control

load_physical_constants

% Get the model
[plant, model] = get_model()

% Number of states 
n_states = size(model.a)(1);
max_acc = motor_out_max*r;
max_aceptable_angle = pi*5/180;
max_aceptable_omega = pi*2/180;
rho = 1;

% Check controlability
co =  ctrb(model);
if (rank(co) > n_states)
  disp(" -> Error! System isn't controllable");
  return;
else
  disp(" -> Great! System is controllable");
endif

ob = obsv(model);
if (rank(ob) > n_states)
  disp(" -> Error! System isn't observable");
  return;
else
  disp(" -> Great! System is observable");
endif

Q = model.c'*model.c

%R = 1/(max_acc^2);
R = 1


[K, X, P] = lqr(model, Q, R);

disp("Control Law")
K

disp("Ricatti Solution")
X

disp("Closed-Loop poles")
P

clf;

Ac = model.a - model.b*K;

sys_cl = ss(Ac, model.b, model.c, model.d)

impulse(sys_cl, 2);



