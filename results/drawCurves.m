function [ ] = drawCurves( filename )
%DRAWCURVE Read a curve from file and plot it
%  

fid = fopen(filename);
n = fscanf(fid, '%u\n', 1);
C = fscanf(fid, '%f %f', [2, Inf]);
fclose(fid);
if size(C, 2) ~= n
    error('Incorrect number of points');
end

%plot(C(1, :)-1.12, C(2, :)-4.787);
plot(C(1,:), C(2, :));
end

