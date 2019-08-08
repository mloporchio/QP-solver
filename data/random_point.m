%
%   File:   random_point.m
%   Author: Matteo Loporchio
%   
%   This function can be used to generate a random point inside
%   the feasible region of a problem.
%

function x = random_point(P)
    A = P.A;
    x = abs(randn(size(A, 2), 1));
    for i = 1:size(A,1) 
        row = A(i, :);
        idx = find(row > 0);
        z = x(idx);
        z = z / sum(z);
        x(idx) = z;
    end
end