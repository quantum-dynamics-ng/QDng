% Find the peaks in a data set
%
% [peaks, mean, sigma] = PeakFinder(data);
% 
% [peaks, mean, sigma] = PeakFinder(data, tolerance);
%
% [peaks, mean, sigma] = PeakFinder(data, tolerance, winsize);
%
% The peak finder returns an array with indices the peaks found in the data set.
% Optionally the mean value of the data set and its standard deviation can be returned.
% If the peaks are not well separated the optional parameter tolerance can be given
% (in units of sigma) to loosen the finding criteria.
% If the data set contains peaks with very different heights than specifing a window size
% may be a better method.
% 
%

