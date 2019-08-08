%
%   File:   modify_problem.m
%   Author: Matteo Loporchio
%   
%   This function can be used to modify the q vector of a problem
%   in order to move the minimum point.
%

function x = modify_problem(name)
    % Load the problem.
    P = load_problem(name);
    % Let x be a random point inside the feasible region of P.
    x = random_point(P);
    % Compute the new vector q.
    q = -2 * P.Q * x;
    % Save the vector to file, possibly replacing the old one.
    writematrix(q, strcat(name, '_u.csv'));
end