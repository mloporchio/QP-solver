%
%   File:       generate_problem_sparse.m
%   Authors:    Matteo Loporchio, Davide Rucci
%
%   This function can be used to generate a problem with a sparse matrix.
%   

function P = generate_problem_sparse(n, k, den)
    % Generate a random positive definite sparse matrix.
    Q = random_sp_matrix(n, den);
    q = randn(n, 1);
    % Generate a random constraint matrix.
    A = random_constr(n, k);
    P = struct('Q', Q, 'q', q, 'A', A);
end