%
%   File:       test_n.m
%   Authors:    Matteo Loporchio, Davide Rucci
%
%   This function can be used to generate test cases with
%   a sparse Hessian and a random size n. These instances are also
%   solved with quadprog to compare the results with our implementation.
%

function test_n(ntest, output_path, output_file)
    ntimes = 5;
    % Open the output file.
    fid = fopen(output_file, 'w');
    for i = 1:ntest
        fprintf('Generating test %d...\n', i);
        n = randi([100 2000], 1, 1);
        k = floor(n/2);
        % Build Q.
        Q = random_sp_matrix(n, 0.999);
        % Build A.
        A = random_constr(n, k);
        % Build q.
        q = randn(n, 1);
        P = struct('Q', Q, 'q', q, 'A', A);
        name = sprintf('test_%d_%d', n, k);
        full_path = sprintf('%s/%s', output_path, name);
        % Solve the problem 5 times.
        [~, v, it, t] = solve_problem_multiple(P, ntimes);
        % Save the data set to disk.
        save_problem(P, full_path, 0);
        % Save the output to CSV file.
        fprintf(fid, sprintf('%d,%d,%f,%f,%f\n', n, k, v, it, t));
    end
    fclose(fid);
end