%
%   File:       load_problem.m
%   Authors:    Matteo Loporchio, Davide Rucci
%
%   This file contains the definition of a function that can
%   be used to import a problem into the current workspace.
%

function P = load_problem(name, sparse)
    q = csvread(strcat(name, '_u.csv'), 0, 0);
    n = size(q, 1);
    if sparse
        Q = load_sp_matrix(strcat(name, '_Q.dat'));
    else
        Q = csvread(strcat(name, '_Q.csv'), 0, 0);
    end
    A = load_constr(strcat(name, '_c.dat'), n);
    P = struct('Q', Q, 'q', q, 'A', A);
end
