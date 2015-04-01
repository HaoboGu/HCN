
[b1 b2] = textread('churn_rate.txt', '%f %f');
[c1 c2 c3 c4 c5 c6 c7 c8 c9 c10 c11 c12 c13 c14 c15 c16 c17 c18 c19 c20 c21]= textread('vacant_scan.txt', '%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f');
[d1 d2 d3 d4 d5 d6 d7 d8 d9 d10 d11 d12 d13 d14 d15 d16 d17 d18 d19 d20 d21] = textread('churn_scan.txt', '%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f');
c = [c1 c2 c3 c4 c5 c6 c7 c8 c9 c10 c11 c12 c13 c14 c15 c16 c17 c18 c19 c20 c21];
d =[d1 d2 d3 d4 d5 d6 d7 d8 d9 d10 d11 d12 d13 d14 d15 d16 d17 d18 d19 d20 d21];
cc = 1- c/370;
factor = 0.6:0.02:1;
rate = 0.0:0.01:0.2;
factor = 1 - factor;
surf(rate, factor, cc);
hold on;
surf(rate, factor, d);

[e1 e2] = textread('ChurnRateChanges2.txt', '%f %f');
[f1 f2] = textread('VacantChanges2.txt', '%f %f');
rate1 = (rate - 0.1)/0.1;
factor1 = (factor - 0.17)/0.17;

e11 = (e1 - 0.35)/0.35;
e22 = (e2 - 0.35)/0.35;
f1 = 370 - f1;
f2 = 370 - f2;
f11 = (f1 - 315)/315;
f22 = (f2 - 315)/315;


plot(rate1,e1);
hold on;
plot(factor1, e2, 'r');
plot(rate1, f1);
hold on;
plot(factor1, f2, 'r');
axis([-1 1 -1 0.6]);

[e1 e2] = textread('ChurnRateChanges1.txt', '%f %f');
e22 = (e2 - 0.35)/0.35;
plot(factor1, e22,'r');hold on;
[e1 e2] = textread('ChurnRateChanges2.txt', '%f %f');
e22 = (e2 - 0.35)/0.35;
plot(factor1, e22, 'b');hold on;
[e1 e2] = textread('ChurnRateChanges3.txt', '%f %f');
e22 = (e2 - 0.35)/0.35;
plot(factor1, e22, 'g');hold on;
axis([-1 1 -1 0.6]);


a = textread('ChurnRate.txt', '%f');
plot(a);
axis([0 50 0 1]);
b = textread('OccupiedRate.txt', '%f');
plot(b);
axis([0 50 0 1]);
c = textread('Productivity.txt', '%f');
plot(c);
axis([0 50 0 500]);
mean(c)

b5 = textread('OccupiedRate5.txt', '%f');
plot(b5);
axis([0 10 0 1]);

a1 = textread('ChurnRate_year1.txt', '%f');
a2 = textread('ChurnRate_year2.txt', '%f');
a3 = textread('ChurnRate_year3.txt', '%f');
plot(a1, 'r');hold on;
plot(a2, 'b');hold on;
plot(a3, 'g');axis([0 20 0 1]);


b1 = textread('OccupiedRate_year1.txt', '%f');
b2 = textread('OccupiedRate_year2.txt', '%f');
b3 = textread('OccupiedRate_year3.txt', '%f');
b4 = textread('OccupiedRate_year4.txt', '%f');
plot(b1, 'r');hold on;
plot(b2, 'b');hold on;
plot(b3, 'g');
plot(b3, 'c');axis([0 20 0.5 1]);

b1 = textread('OccupiedRate5.txt', '%f');
b2 = textread('ChurnRate5.txt', '%f');
plot(b1)
plot(b2)