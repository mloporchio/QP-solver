%
%   File:       save_problem.m
%   Authors:    Matteo Loporchio, Davide Rucci
%   
%   This function can be used to save a problem to disk.
%

function save_problem(P, name, sparse)
    if sparse
        save_sp_matrix(P.Q, strcat(name, '_Q.dat'))
    else
        writematrix(P.Q, strcat(name, '_Q.csv'));
    end
    writematrix(P.q, strcat(name, '_u.csv'));
    save_constr(P.A, strcat(name, '_c.dat'));
end