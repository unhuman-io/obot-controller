data = readtable('mpstest.txt');

%%
i_a = data.Var1;
i_b = data.Var2;
i_c = data.Var3;
v_a = data.Var4;
v_b = data.Var5;
v_c = data.Var6;
i_am = data.Var7;
i_bm = data.Var8;
i_cm = data.Var9;

figure(1);
plot([i_a,i_b,i_c])
title('allegro current')
print -dpng -r200 allegro.png

figure(2);
plot([v_a,v_b,v_c])

figure(3);
plot([i_am,i_bm,i_cm])
title('mps current')
print -dpng -r200 mps.png

figure(4);
plot(i_a,-i_am,i_b,-i_bm, i_c, -i_cm);
legend('i_a','i_b','i_c')
xlabel('allegro adc')
ylabel('mps adc')
print -dpng -r200 vs.png

figure(5);
plot(v_a,i_a,v_b,i_b)

figure(6);
plot(v_a,i_am,v_b,i_bm)