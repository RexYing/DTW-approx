function [ C ] = compressCurve( inputFile, dir, r, outputFile )
%COMPRESSCURVE Compress curve in x or y direction
%   
%   If dir='x', compress curve in x direction
%   If dir='y', compress curve in y direction
%   r: the compress ratio. The resulting curve will be r times shorter in
%   x/y axis
%

fid = fopen(inputFile);
n = fscanf(fid, '%u\n', 1);
C = fscanf(fid, '%f %f', [2, Inf]);
fclose(fid);
if size(C, 2) ~= n
    error('Incorrect number of points');
end

C(1, :) = C(1, :) / r;

%% write curve
fid = fopen(outputFile, 'w');
fprintf(fid, '%u\n', length(C));
fprintf(fid, '%f %f\n', C);
fclose(fid);

end

