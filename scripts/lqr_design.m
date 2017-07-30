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

Q = model.c'*model.c;

Q

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
sys_cl = ss(Ac, model.b, model.c, model.d);
figure(1);
clf(1)
impulse(sys_cl, 2);

disp("Slowest pole")
s_p = 1000;
for i = 1:size(P)(1)
  if (abs(real(P(i))) < abs(real(s_p)))
    s_p = P(i);
  endif
endfor
s_p

disp("Observer's poles")
for i = 1:size(model.a)(1)
  o_p(i) = (10 + i)*real(s_p) + imag(s_p);
endfor
o_p

disp("Observer matrix")
L = place(model.a',model.c',o_p)'

disp("Full compensator model")
Ace = [(model.a-model.b*K) (model.b*K);
        zeros(size(model.a)) (model.a-L*model.c)];
Bce = [model.b; % N matrix is 1 since our reference is consider to be 0
        zeros(size(model.b))];
Cce = [model.c zeros(size(model.c))];
Dce = [0;0;0];

fcss = ss(Ace, Bce, Cce, Dce)
figure(2);
clf(2)
impulse(fcss, 2)




