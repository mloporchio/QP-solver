%
%   File:   test.m
%   Author: Matteo Loporchio
%
%   In this file we try to solve our problem using Matlab's
%   quadprog solver. The solutions can be used as a benchmark
%   to assess the quality of our implementation.
%

function v = test(id)
    name = sprintf('test%d', id);
    Q = csvread(strcat(name, '_Q.csv'), 0, 0);
    A = csvread(strcat(name, '_A.csv'), 0, 0);
    u = csvread(strcat(name, '_u.csv'), 0, 0);
    b = ones(size(A, 1), 1);
    z = zeros(size(Q, 1), 1);
    [~, val] = quadprog(2 * Q, u, [], [], A, b, z, []);
    v = val;
end
