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

figure
drawCurves('data/GeoLife/005/20090119101553.txt');
hold on
drawCurves('data/GeoLife/005/20090208045522.txt');
title('Example trajectories');

%% example curves
figure
underlyingParamCurves;
hold on
underlyingParamCurves;

%% DTW matrix

visualizeRects('results/rects_rand_5_6.txt');
hold on
plotAlignment('results/rects_rand_5_6_align.txt', 'r');
plotAlignment('results/rects_rand_5_6_exact_align.txt', 'b');
plotAlignment('results/rects_rand_5_6_approx_frechet_align.txt', 'y');
legend('approximate DTW alignment', 'exact DTW alignment', 'approximate Frechet alignment');
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

visualizeRects('results/trajectories/20090119101553_20090208045522_translated_1.txt');
hold on
plotAlignment('results/trajectories/20090119101553_20090208045522_translated_1_approx_align.txt', 'r');
plotAlignment('results/trajectories/20090119101553_20090208045522_translated_1_exact_align.txt', 'b');
%plotAlignment('results/rects_rand_3_7_approx_frechet_align.txt', 'y');
legend('approximate DTW alignment', 'exact DTW alignment');
hold off
title('Rectangles and alignments of two curves that differ in step size');

%% 3D DTW table
visualizeRectValues('results/rects_line_1_2.txt', 'data/line1.dtw', 'data/line2.dtw');

visualizeRectValues('results/rects_rand_3_7.txt', 'data/rand3.dtw', 'data/rand7.dtw');

visualizeRectValues('results/rects_rand_5_6.txt', 'data/rand5.dtw', 'data/rand6.dtw');

%%

% varying eps
eps = [0.3 0.5 0.6 0.7 0.8 0.9 1.0 1.5 2];

% rand looping curve
deviation = [0.05235, 0.046786, 0.053291, 0.07195, 0.0836992, 0.0759031, 0.0885458, 0.0968, 0.112];
bpts = [7025817, 4346380, 4141482, 3724512, 2635669, 1912600, 1750080, 1283941, 934485] / 5;
rect = [165227, 56896, 51384, 40701, 19230, 10032, 7980];

figure
plot(eps, deviation, 'r');
title('Actual error vs. epsilon');

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
