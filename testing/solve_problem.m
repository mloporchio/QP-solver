%
%   File:       solve_problem.m
%   Authors:    Matteo Loporchio, Davide Rucci
%   
%   In this file we try to solve our problem using Matlab's
%   quadprog solver. The solutions can be used as a benchmark
%   to assess the quality of our implementation.
%

function [x, v, it, t] = solve_problem(P)
    b = ones(size(P.A, 1), 1);
    z = zeros(size(P.Q, 1), 1);
    % Time the execution of quadprog.
    tic;
    [x, v, ~, out] = quadprog(2 * P.Q, P.q, [], [], P.A, b, z, []);
    t = toc * 1000;
    it = out.iterations;
end

