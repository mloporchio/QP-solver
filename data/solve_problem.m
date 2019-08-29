%
%   File:   solve_problem.m
%   Author: Matteo Loporchio
%
%   In this file we try to solve our problem using Matlab's
%   quadprog solver. The solutions can be used as a benchmark
%   to assess the quality of our implementation.
%

function [x, v, it] = solve_problem(P)
    b = ones(size(P.A, 1), 1);
    z = zeros(size(P.Q, 1), 1);
    [x, v, ~, out] = quadprog(2 * P.Q, P.q, [], [], P.A, b, z, [], [], []);
    it = out.iterations;
end
