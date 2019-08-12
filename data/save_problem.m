%
%   File:   save_problem.m
%   Author: Matteo Loporchio
%   
%   This function can be used to save a problem to disk.
%

function save_problem(P, name)
    writematrix(P.Q, strcat(name, '_Q.csv'));
    writematrix(P.q, strcat(name, '_u.csv'));
    save_constr(P.A, strcat(name, '_c.txt'));
end