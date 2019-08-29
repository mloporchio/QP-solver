%
%   File:   iteration_sparse.m
%   Author: Matteo Loporchio
%

function iteration_sparse(path)
    % Obtain the list of the data sets to be loaded.
    files = dir(fullfile(path, '*_Q.dat'));
    % Open the output file.
    fid = fopen('output.csv', 'w');
    % For each data set...
    for i = 1:length(files)
        data_set = erase(files(i).name, '_Q.dat');
        full_path = strcat(path, data_set);
        fprintf('Loading problem: %s\n', full_path);
        P = load_problem(full_path, 1);
        fprintf('Problem loaded!\n');
        [~, v, it] = solve_problem(P);
        fprintf(fid, sprintf('%s,%f,%d\n', data_set, v, it));
    end
end