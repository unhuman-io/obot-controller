%d = readtable("~/nuc8/tmpfs/tmp.csv");
figure(1)
plot(d.t_seconds0, [d.torque0,d.reserved00])
grid

figure(2)
plot(diff(d.t_seconds0))

figure(3)
plot(diff(d.t_host))

figure(4)
plot(d.t_seconds0,[d.iq0, d.torque0])

figure(5)
plot(d.t_seconds0,[d.iq0,d.motor_position0-mean(d.motor_position0)])