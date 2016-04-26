function [ ] = affineSin( n )
%AFFINE_SIN Summary of this function goes here
%   Detailed explanation goes here


n = 3140;
t = (1: n) / 300;
y = sin(t);


theta = -1/15 * pi;
P = [t', y'] * [1, 0.3; 0.3, 1] * [cos(theta), sin(theta); -sin(theta), cos(theta)];
t1 = P(:, 1);
y1 = P(:, 2);

theta = 1/6 * pi;
P = [t', y'] * [1, -0.6; -0.6, 1] * [cos(theta), sin(theta); -sin(theta), cos(theta)];
t2 = P(:, 1);
y2 = P(:, 2);

figure
%plot(t, y);
%hold on
plot(t1, y1);
hold on
plot(t2, y2);

end

