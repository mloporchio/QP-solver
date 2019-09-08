%
%   File:       load_constr.m
%   Authors:    Matteo Loporchio, Davide Rucci
%
%   This function reads the constraints file and generates the
%   k x n constraint matrix with {0, 1} entries.
%

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