# Consants (context)
l = 0.0423;
m =  0.24200;
g =  9.8100;
b = 0.5;
K_s = 0.012;
K_uc = 60;

pkg load control

# Create the system for the plant
plant = tf([-K_s 0], [l 0 -g])
figure(1)
rlocus(plant)

# Create the PID controller
cont = pid(40, 1, 5);

# Get the rlocus of the system + controller
cascade = cont * plant;
figure(2)
#rlocus(cascade);

# Get the close loop system
fbl = feedback(cascade, 1)
ss(fbl)

figure(3)
step(ss(fbl), 5);

# Get poles and zeros
pole(fbl)
zero(fbl)