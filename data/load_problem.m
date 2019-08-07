%
%   File:   load_problem.m
%   Author: Matteo Loporchio
%
%   This file contains the definition of a function that can
%   be used to import a problem into the current workspace.
%

function P = load_problem(name)
    Q = csvread(strcat(name, '_Q.csv'), 0, 0);
    q = csvread(strcat(name, '_u.csv'), 0, 0);
    A = csvread(strcat(name, '_A.csv'), 0, 0);
    P = struct('Q', Q, 'q', q, 'A', A);
end
