function [  ] = visualizeRects( filename)
%VISUALIZERECTS Visualize rectangles
%   
% Given a file containing a 2D matrix for dp, with '.' representing
% boundary points and ' ' otherwise, visualize it as a binary image
%
% Optional param:
%   alignmentFilename: the filename of file that contains alignment info.
%       The file has to contain a pair of integers each line.
%

fid = fopen(filename);

s = fscanf(fid, '%u %u\n', 2);
%DP = sparse(s(1), s(2));
DP = ones(s(1), s(2)) / 2;

rects = fscanf(fid, '%f %f %f %f', [4, Inf]);
fclose(fid);

for i = 1: size(rects, 2)
    r = [rects(1, i) + 1, rects(2, i)];
    c = [rects(3, i) + 1, rects(4, i)];
    DP(r(1): r(2), c(1)) = 0;
    DP(r(1): r(2), c(2)) = 0;
    DP(r(1), c(1): c(2)) = 0;
    DP(r(2), c(1): c(2)) = 0;
    DP(r(1) + 1: r(2) - 1, c(1) + 1: c(2) - 1) = 1;
end

figure
imshow(DP);

end
