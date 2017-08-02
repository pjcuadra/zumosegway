clear;
clc;

load_physical_constants

% Max acceptable values per state and input
max_motor_angular_position = 2 * 2 * pi; % 2 complete cycles
max_zumo_angular_position = 5 * 2 * pi / 180; % 5 Degrees
max_motor_angular_speed = 4 * 2 * pi; % RPS (measured)
max_zumo_angular_speed = 1 * 2 * pi / 180; % 1 DPS
max_input = 400;
rho = 1;

% Get the model
[plant, model] = get_model()

% Number of states 
n_states = size(model.a, 1);

% Check controlability
co =  ctrb(model);
if (rank(co) > n_states)
  disp(" -> Error! System isn't controllable");
  return;
else
  disp(" -> Great! System is controllable");
end

ob = obsv(model);
if (rank(ob) > n_states)
  disp(" -> Error! System isn't observable");
  return;
else
  disp(" -> Great! System is observable");
end

Q = eye(size(model.a,1));

R = 1

[K, X, P] = lqr(model, Q, R);

K = K*pi/180;

disp("Control Law")
K_string = "{";
for k = 1:size(K, 2)
  if ~(k == 1) 
    K_string = strcat(K_string, ", ");
  end
  K_string = strcat(K_string, num2str(K(k)));
end
K_string = strcat(K_string, "}");
disp("K")
disp(K_string)

disp("Ricatti Solution")
X

disp("Closed-Loop poles")
P

clf;

Ac = model.a - model.b*K;
sys_cl = ss(Ac, model.b, model.c, model.d);
figure(1);
clf(1)
impulse(sys_cl, 3);

disp("Skipping Observer design ...");
return;

disp("Slowest pole")
s_p = 1000;
for i = 1:size(P, 1)
  if (abs(real(P(i))) < abs(real(s_p)))
    s_p = P(i);
  end
end
s_p

disp("Observer's poles")
for i = 1:size(model.a, 1)
  o_p(i) = (10 + i)*real(s_p) + imag(s_p);
end
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




