%
%   File:       load_sp_matrix.m
%   Author:     Matteo Loporchio
%
%   This function can be used to load a sparse matrix S
%   using IJV or coordinate (COO) format. 
%

function S = load_sp_matrix(file, n)
    fid = fopen(file);
    tline = fgetl(fid);
    % Create an empty n x n sparse matrix.
    S = sparse(n, n);
    while ischar(tline)
        L = strsplit(tline);
        i = str2double(L{1,1}) + 1;
        j = str2double(L{1,2}) + 1;
        v = str2double(L{1,3});
        S(i, j) = v;
        tline = fgetl(fid);
    end
    fclose(fid);
end