%
%   File:       variable_density.m
%   Authors:    Matteo Loporchio, Davide Rucci
%
%   This function can be used to generate tests with increasing density.
%   The test cases are also immediately solved by quadprog 
%   in order to compare the results with our implementation.
%

function variable_density(n, ntest, output_path, output_file)
    % Generate, solve and save the test cases.
    den = linspace(0.01, 1.00, ntest);
    ntimes = 5;
    % Open the output file.
    fid = fopen(output_file, 'w');
    for i = 1:ntest
        fprintf('Generating test %d...\n', i);
        k = floor(n/2);
        P = generate_problem_sparse(n, k, den(i));
        name = sprintf('test_%d_%d_%.3f', n, k, den(i));
        full_path = sprintf('%s/%s', output_path, name);
        % Solve the problem 5 times.
        [~, v, it, t] = solve_problem_multiple(P, ntimes);
        % For matrices with density <= 0.5, we use the sparse data
        % structure.
        if den(i) <= 0.5
            save_problem(P, full_path, 1);
        else
            save_problem(P, full_path, 0);
        end
        save_problem(P, full_path, 1);
        fprintf(fid, sprintf('%d,%d,%f,%f,%f,%f\n', n, k, den(i), ...
        v, it, t));
    end
    fclose(fid);
end
