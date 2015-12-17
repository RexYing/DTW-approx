figure
drawCurves('data/line1.dtw');
hold on
drawCurves('data/line2.dtw');

figure
drawCurves('data/rand1.dtw');
hold on
drawCurves('data/rand2.dtw');

figure
drawCurves('data/rand3.dtw');
hold on
drawCurves('data/rand4.dtw');

%% DTW matrix

visualizeRects('results/rects1.txt');

visualizeRects('results/rects2.txt');


%%

% varying eps
eps = [0.3 0.5 0.6 0.7 0.8 0.9 1.0];

% rand looping curve
deviation = [0.02535, 0.016786, 0.013291, 0.09195, 0.126992, 0.139031, 0.115458];
bpts = [7025817, 4346380, 4141482, 3724512, 2635669, 1912600, 1750080];
rect = [165227, 56896, 51384, 40701, 19230, 10032, 7980];

figure
subplot(1, 3, 1)
plot(eps, deviation, 'r');

subplot(1, 3, 2)
plot(eps, bpts, 'b');

subplot(1, 3, 3)
plot(eps, rect, 'm');


% varying n
n = [500, 1000, 2000, 3000, 5000, 7000, 9000, 12000];
approx_time = [0.451867, 0.84096, 3.02536, 4.2342, 7.08304, 11.67045, 16.0546, 23.84];
naive_time = [0.004885, 0.017794, 0.065383, 0.147485, 0.41084, 0.8012, 1.546, 2.945];
figure
title('Time against n');
plot(n, approx_time / 10);
hold on
plot(n, naive_time);
legend('approx', 'naive');
