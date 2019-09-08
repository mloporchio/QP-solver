%
%   File:       save_constr.m
%   Authors:    Matteo Loporchio, Davide Rucci
%   
%   This function writes a constraint matrix to a file.
%   Since the matrix is sparse, we only write the indices of the
%   non-zero elements. The output file has k rows (one for each row
%   of the matrix).
%

function save_constr(A, file)
    % Open the output file.
    fid = fopen(file, 'w');
    % For each row of the matrix...
    for i = 1:size(A, 1)
        row = A(i, :);
        % Find the non-zero components.
        idx = find(row > 0);
        % Convert the vector to a printable string.
        str = to_string(idx - 1);
        % Write the string to file.
        fprintf(fid, sprintf('%s\n', str));
    end
    fclose(fid);
end

% Returns a string that represents a vector.
% Elements are listed using commas as separators.
function s = to_string(v)
    s = sprintf('%d,', v);
    s = s(1:end-1);
end