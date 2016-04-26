function [  ] = visualizeRectValues( rectFile,  curveFile1, curveFile2)
%VISUALIZERECTVALUES Visualize values of rectangles as 3D plot
%
% The function generates 2 plots: one corresponds to the values of original
% DP table; the other corresponds to the values of approximate DP table
% using rectangles.
%
% Input: 
%   rectFile: file containing information of rectangles
%   curveFile1, curveFile2: file containing input sequences
%
%
%

%% Read input

fid = fopen(rectFile);

s = fscanf(fid, '%u %u\n', 2);
%DP = sparse(s(1), s(2));

rects = fscanf(fid, '%f %f %f %f', [4, Inf]);
fclose(fid);

fid = fopen(curveFile1);
n = fscanf(fid, '%u\n', 1);
c1 = fscanf(fid, '%f %f', [2, Inf]);
fclose(fid);
if size(c1, 2) ~= n
    error('Incorrect number of points');
end

fid = fopen(curveFile2);
n = fscanf(fid, '%u\n', 1);
c2 = fscanf(fid, '%f %f', [2, Inf]);
fclose(fid);
if size(c2, 2) ~= n
    error('Incorrect number of points');
end

%% Compute original DP values

DP = zeros(s(1), s(2));
for i = 1: s(1)
    for j = 1: s(2)
        DP(i, j) = norm(c1(:, i) - c2(:, j));
    end
end

figure
surf(DP(1:10:s(1), 1:10:s(2)), 'linestyle', 'none');

%% Compute approx DP values
DP = ones(s(1), s(2)) / 2;

for i = 1: size(rects, 2)
    r = [rects(1, i) + 1, rects(2, i)];
    c = [rects(3, i) + 1, rects(4, i)];
    v = norm(c1(:, r(1)) - c2(:, c(1)));
    DP(r(1): r(2), c(1): c(2)) = v;
end

figure
surf(DP(1:10:s(1), 1:10:s(2)), 'linestyle', 'none');

end

