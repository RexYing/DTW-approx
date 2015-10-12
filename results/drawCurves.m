function [ ] = drawCurves( filename )
%DRAWCURVE Read a curve from file and plot it
%  

fid = fopen(filename);
n = fscanf(fid, '%u\n', 1);
C = fscanf(fid, '%f %f', [2, Inf]);

plot(C(1, :), C(2, :));

end

