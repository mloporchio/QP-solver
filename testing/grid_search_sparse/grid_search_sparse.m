%
%   File:   grid_search_sparse.m
%   Author: Matteo Loporchio
%
%   This function processes all sparse data sets and solves them
%   using quadprog. The results are written to a CSV file.
%

function grid_search_sparse(path)
    % Obtain the list of the data sets to be loaded.
    files = dir(fullfile(path, '*_Q.dat'));
    % Open the output file.
    fid = fopen('grid_search_sparse_matlab.csv', 'w');
    % For each data set...
    for i = 1:length(files)
        % Obtain the name of the data set.
        data_set = erase(files(i).name, '_Q.dat');
        full_path = strcat(path, data_set);
        % Check the density.
        s = strsplit(data_set, '_');
        den = str2double(erase(s(4), 'd'));
        % Load and solve the problem.
        fprintf('Processing data set: %s\n', full_path);
        if (den <= 0.5)
            P = load_problem(full_path, 1);
        else
            P = load_problem(full_path, 0);
        end
        [~, v, it, t] = solve_problem(P);
        % Obtain the dimensions of the problem.
        k = size(P.A, 1);
        n = size(P.A, 2);
        % Write the result to file.
        fprintf(fid, sprintf('%d,%d,%f,%f,%d,%f\n', n, k, den, v, it, t));
    end
    fclose(fid);
end