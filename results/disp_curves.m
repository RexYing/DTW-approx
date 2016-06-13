figure
drawCurves('data/line1.dtw');
hold on
drawCurves('data/line2.dtw');

figure
drawCurves('data/rand1.dtw');
hold on
drawCurves('data/rand2.dtw');

figure
drawCurves('data/GeoLife/005/20090119101553.txt');
hold on
drawCurves('data/GeoLife/005/20090208045522.txt');
title('Example trajectories');

compressCurve('data/rand3.dtw', 'x', 100, 'data/input4_1.txt');
compressCurve('data/rand7.dtw', 'x', 100, 'data/input4_2.txt');

figure
drawCurves('data/input4_1.txt');
hold on
drawCurves('data/input4_2.txt');

%% example curves
figure
underlyingParamCurves;
hold on
underlyingParamCurves;

x = 1: 2000;
gpdf = normpdf((x-1000)/500, 0, 1)';
noiseCov = diag([0.00000001, 0.0000001]);
C1 = samplePerturbedCurve('data/GeoLife/005/20090119101553.txt', 'data/GeoLife/005/20090119101553_s1.txt', gpdf, noiseCov);
C2 = samplePerturbedCurve('data/GeoLife/005/20090119101553.txt', 'data/GeoLife/005/20090119101553_s2.txt', 1/4, noiseCov);
figure; 
plot(C1(1, :), C1(2, :));
hold on
plot(C2(1, :), C2(2, :));


%% DTW matrix

visualizeRects('results/rects_rand_5_6.txt');
hold on
plotAlignment('results/rects_rand_5_6_align.txt', 'r');
plotAlignment('results/rects_rand_5_6_exact_align.txt', 'b');
plotAlignment('results/rects_rand_5_6_exact_frechet_align.txt', 'y');
legend('approximate DTW alignment', 'exact DTW alignment', 'exact Frechet alignment');
hold off
title('Rectangles and alignments of two curves that have random step sizes and angles');

visualizeRects('results/rects_rand_3_7.txt');
hold on
plotAlignment('results/rects_rand_3_7_align.txt', 'r');
plotAlignment('results/rects_rand_3_7_exact_align.txt', 'b');
plotAlignment('results/rects_rand_3_7_approx_frechet_align.txt', 'y');
legend('approximate DTW alignment', 'exact DTW alignment', 'approximate Frechet alignment');
hold off
title('Rectangles and alignments of two curves that differ in step size');

visualizeRects('results/trajectories/20090119101553_20090208045522_translated.txt');
hold on
plotAlignment('results/trajectories/20090119101553_20090208045522_translated_approx_align.txt', 'r');
plotAlignment('results/trajectories/20090119101553_20090208045522_translated_exact_align.txt', 'b');
%plotAlignment('results/rects_rand_3_7_approx_frechet_align.txt', 'y');
legend('approximate DTW alignment', 'exact DTW alignment');
hold off
title('Rectangles and alignments of two curves that differ in step size');

%% 3D DTW table
visualizeRectValues('results/rects_line_1_2.txt', 'data/line1.dtw', 'data/line2.dtw');

visualizeRectValues('results/rects_input4.txt', 'data/input4_1.txt', 'data/input4_2.txt');

visualizeRectValues('results/rects_rand_5_6.txt', 'data/rand5.dtw', 'data/rand6.dtw');

visualizeRectValues('results/trajectories/20090119101553_20090208045522_translated.txt', 'data/GeoLife/005/20090119101553.txt', 'data/GeoLife/005/20090208045522_translated.txt');

visualizeRectValues('results/trajectories/20090119101553_s1_s2.txt', 'data/GeoLife/005/20090119101553_s1.txt', 'data/GeoLife/005/20090119101553_s2.txt');

%%

% varying eps
eps = [0.3 0.5 0.6 0.7 0.8 0.9 1.0 1.5 2];

time = [ 6.535, 2.89, 2.2885,  1.631, 1.3997, 1.07698, 0.380915, 0.1998, 0.2022];
timeStd = [0.5, 0.32, 0.36, 0.25, 0.22, 0.23, 0.18, 0.11, 0.09];

figure
errorbar(eps, time, timeStd);

% rand looping curve
deviation = [0.05235, 0.056786, 0.059291, 0.07195, 0.0936992, 0.0989031, 0.1025458, 0.1468, 0.27];
bpts = [7025817, 4346380, 4141482, 3724512, 2635669, 1912600, 1750080, 1283941, 934485] / 5;
rect = [165227, 56896, 51384, 40701, 19230, 10032, 7980];

figure
plot(eps, deviation, 'r');
title('Actual error vs. epsilon');
xlabel('epsilon');
ylabel('actual approximation error');

figure
plot(eps, bpts, 'b');
title('Number of boundary points vs. epsilon');

figure
plot(eps, rect, 'm');


% varying n
n = [500, 1000, 2000, 3000, 5000, 7000, 9000, 12000, 15000, 20000];
approx_time = [0.451867, 0.84096, 3.02536, 4.2342, 7.08304, 11.67045, 16.0546, 23.84, 31, 44.5] * 2;
naive_time = [0.004885, 0.017794, 0.065383, 0.147485, 0.41084, 0.8012, 1.546, 2.945, 5.12, 11.3]*2;
new_time_box = [0.024885, 0.049794, 0.080383, 0.127485, 0.27084, 0.5212, 0.886, 1.22, 1.61, 2.35];
new_time_ellipse = [0.025885, 0.045794, 0.091383, 0.121485, 0.23084, 0.4412, 0.712, 1.13, 1.40, 2.08];

figure
title('Time against n');
plot(n, approx_time / 8);
hold on
plot(n, naive_time);
plot(n, new_time_box);
plot(n, new_time_ellipse);
title('DTW running time comparison');

legend('approximation algorithm with overlapping rectangles', 'naive', 'disjoint rectangles, bounding box', 'disjoint rectangles, bounding ellipse');
