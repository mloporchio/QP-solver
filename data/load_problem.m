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
    A = load_constr(strcat(name, '_c.txt'), size(Q, 1));
    P = struct('Q', Q, 'q', q, 'A', A);
end

%   This function reads the constraints file and generates the
%   k x n constraint matrix with {0, 1} entries.
function M = load_constr(file, n)
    M = [];
    fid = fopen(file);
    tline = fgetl(fid);
    while ischar(tline)
        v = zeros(n, 1);
        idx = 1 + str2num(tline);
        v(idx) = 1;
        M(end + 1, :) = v;
        tline = fgetl(fid);
    end
end
