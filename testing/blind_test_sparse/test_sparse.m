%
%   File:       test_sparse.m
%   Authors:    Matteo Loporchio, Davide Rucci
%
%   This function can be used to run tests for sparse data sets.
%   The function loads the data sets from their files and solves each
%   one of them several times. The output is the average of the
%   running times and number of iterations.

function test_sparse(path, output)
    % Obtain the list of the data sets to be loaded.
    files = dir(fullfile(path, '*_c.dat'));
    % Open the output file.
    fid = fopen(output, 'w');
    % Compute the number of data sets.
    nd = length(files);
    % Number of times to solve each problem.
    ntimes = 5;
    for i = 1:nd
        data_set = erase(files(i).name, '_c.dat');
        full_path = sprintf('%s/%s', path, data_set);
        split = strsplit(data_set, '_');
        fprintf('Loading problem %d/%d...\n', i, nd);
        n = str2double(split{2});
        k = str2double(split{3});
        den = str2double(split{4});
        fprintf(full_path);
        if den <= 0.5
            P = load_problem(full_path, 1);
        else
            P = load_problem(full_path, 0);
        end
        [~, v, it, t] = solve_problem_multiple(P, ntimes);
        fprintf(fid, sprintf('%d,%d,%f,%f,%f,%f\n', n, k, den, v, it, t));
    end
    fclose(fid);
end