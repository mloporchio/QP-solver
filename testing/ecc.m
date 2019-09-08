%
%   File:       ecc.m
%   Authors:    Matteo Loporchio, Davide Rucci
%   
%   This function computes the eccentricity of a n * n matrix.
%   It is defined as:
%
%       e = (l1 - ln) / (l1 + ln)
%   
%   where l1 is the largest eigenvalue and ln is the smallest one.
%

function e = ecc(X)
    n = size(X, 1);
    % Sort the eigenvalues of X in ascending order.
    v = sort(eig(X));
    smallest = v(1);
    largest = v(n);
    e = (largest - smallest) / (largest + smallest);
end