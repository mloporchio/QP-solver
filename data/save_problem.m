%
%   File:   save_problem.m
%   Author: Matteo Loporchio
%   
%   This function can be used to save a problem to disk.
%   The problem is saved into three different CSV files.
%

function save_problem(P, name)
    writematrix(P.Q, strcat(name, '_Q.csv'));
    writematrix(P.q, strcat(name, '_u.csv'));
    writematrix(P.A, strcat(name, '_A.csv'));
end