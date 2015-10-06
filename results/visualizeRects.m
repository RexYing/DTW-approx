function [ output_args ] = visualizeRects( filename )
%VISUALIZERECTS Visualize rectangles
%   
% Given a file containing a 2D matrix for dp, with '.' representing
% boundary points and ' ' otherwise, visualize it as a binary image
%

fid = fopen(filename);

tline = fgets(fid);
while ischar(tline)
    n = length(tline);
    
    tline = fgets(fid);
end

fclose(fid);

end

