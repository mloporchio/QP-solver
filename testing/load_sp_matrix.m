%
%   File:       load_sp_matrix.m
%   Authors:    Matteo Loporchio, Davide Rucci
%
%   This function can be used to load a sparse matrix S
%   using IJV or coordinate (COO) format. 
%

function S = load_sp_matrix(file)
    fid = fopen(file);
    tline = fgetl(fid);
    k = 1;
    i = [];
    j = [];
    v = [];
    while ischar(tline)
        A = sscanf(tline, '%f', [3 1]);
        i(k) = A(1) + 1;
        j(k) = A(2) + 1;
        v(k) = A(3);
        k = k + 1;
        tline = fgetl(fid);
    end
    S = sparse(i, j, v);
    fclose(fid);
end