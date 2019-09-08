%
%   File:       random_sp_matrix.m
%   Authors:    Matteo Loporchio, Davide Rucci
%
%   This function can be used to generate a n x n sparse
%   positive semidefinite matrix with a given density.
%

function Q = random_sp_matrix(n, den)
    Q = sprandsym(n, den);
    Q = Q + n * speye(n);
end
