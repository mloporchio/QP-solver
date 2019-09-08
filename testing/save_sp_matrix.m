%
%   File:       save_sp_matrix.m
%   Authors:    Matteo Loporchio, Davide Rucci
%
%   This function can be used to save a sparse matrix S
%   using IJV or coordinate (COO) format. 
%   For every non-zero entry Sij we print a line:
%
%       i j v
%
%   where v is the non-zero value in S at row i, column j.

function save_sp_matrix(S, name)
    [i,j,v] = find(S);
    fid = fopen(name, 'wt');
    fprintf(fid, '%d %d %g\n', [i-1,j-1,v]');
    fclose(fid);
end
