function [ C ] = samplePerturbedCurve( inputFile, outputFile, p, noiseCov )
%SAMPLEPERTURBEDCURVE Sample a curve and add perturbation
%   
%   If p is a number, each point on the curve is sampled with probability p
%   If p is a vector, i-th point on the curve is sampled with probability
%   p(i)
%
%   noiseCov: the covariance matrix of gaussian noise.
%

fid = fopen(inputFile);
n = fscanf(fid, '%u\n', 1);
C = fscanf(fid, '%f %f', [2, Inf]);
fclose(fid);
if size(C, 2) ~= n
    error('Incorrect number of points');
end

N = ones(n, 1);
if (length(p) > 1)
    samples = binornd(N, p(1: n));
else
     samples = binornd(N, p);
end
C = C(:, logical(samples));
noise = mvnrnd([0, 0], noiseCov, length(C))';
C = C + noise;

%% write curve
fid = fopen(outputFile, 'w');
fprintf(fid, '%u\n', length(C));
fprintf(fid, '%f %f\n', C);
fclose(fid);

end

