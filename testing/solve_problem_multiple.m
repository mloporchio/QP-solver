%
%   File:       solve_problem_multiple.m
%   Authors:    Matteo Loporchio, Davide Rucci
%
%   This function can be used to estimate the running time of quadprog
%   by executing it multiple times.
%

function [x, v, it, t] = solve_problem_multiple(P, n)
    b = ones(size(P.A, 1), 1);
    z = zeros(size(P.Q, 1), 1);
    % Execute quadprog n times.
    it = 0;
    t = 0;
    for i = 1:n
        tic;
        [x, v, ~, out] = quadprog(2 * P.Q, P.q, [], [], P.A, b, z, []);
        t = t + (toc * 1000);
        it = it + out.iterations;
    end
    t = t / n;
    it = it / n;
end