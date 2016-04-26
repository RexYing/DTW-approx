function [ output_args ] = plotAlignment( alignmentFilename, color )
%PLOTALIGNMENT Plot alignment as a path in the DP table
%  

fid = fopen(alignmentFilename);
alignment = fscanf(fid, '%u %u\n', [2, Inf]);
hold on
plot(alignment(2, :) +1, alignment(1, :) +1, 'Color', color, 'LineWidth', 2);
 
% plot pixels
% figure
% for i = 1: size(alignment, 2)
%     if DP(alignment(1, i) + 1, alignment(2, i) + 1) == 1
%         fprintf('%d %d\n', alignment(1, i) + 1, alignment(2, i) + 1 );
%     end
%     DP(alignment(1, i) + 1, alignment(2, i) + 1) = 0.5;
% end
% imshow(DP);

end

