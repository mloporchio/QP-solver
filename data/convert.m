%
%   File:   convert.m
%   Author: Matteo Loporchio
%   
%   This function can be used to convert a problem whose constraints
%   are still represented using the full constraint matrix.
%   The function will produce a new text file where the constraints
%   are represented according to the new format.
%

function convert(name)
    A = csvread(strcat(name, '_A.csv'), 0, 0);
    save_constr(A, strcat(name, '_c.txt'));
end