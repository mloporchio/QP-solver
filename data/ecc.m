%
%   File:   ecc.m
%   Author: Matteo Loporchio
%   
%   This function computes the eccentricity of a n * n matrix.
%   It is defined as:
%
%       e = (l_1 - l_n) / (l_1 + l_n)
%   
%   where l_1 is the largest eigenvalue and l_n is the smallest one.
%

function e = ecc(X)
    n = size(X, 1);
    % Compute the eigenvalue decomposition of X.
    [~, D] = eig(X);
    l = diag(D);
    e = (l(1) - l(n)) / (l(1) + l(n));
end