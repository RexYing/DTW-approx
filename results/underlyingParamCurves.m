function [ ] = drawExampleCurves(  )
%DRAWCURVE Read a curve from file and plot it
%  

n = 628;
t = 1: n;
a = 0.5;
x = (a + cos(3*t / 100)) .* cos(t / 100) * 100;
y = (a + cos(3*t / 100)) .* sin(t / 100) * 100;
for i = 1: n
    noise = mvnrnd([0 0], diag([0.1, 0.1]));
    %noise = [0.1 0];
    x(i) = x(i) + noise(1);
    y(i) = y(i) + noise(2);
end

plot(x, y);


end

