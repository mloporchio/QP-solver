%
%   File:       test_sparse_generator.m
%   Authors:    Matteo Loporchio, Davide Rucci
%
%   This function can be used to generate random test cases 
%   for the sparse blind tests.
%

function test_sparse_generator(path, ntest)
    % Generate and save the test cases.
    for i = 1:ntest
        fprintf('Generating test %d...\n', i);
        n = randi([100 2000], 1, 1);
        k = randi([1 n], 1, 1);
        den = randi([1 100], 1, 1) / 100;
        P = generate_problem_sparse(n, k, den);
        name = sprintf('test_%d_%d_%.2f', n, k, den);
        full_path = sprintf('%s/%s', path, name);
        % For matrices with density <= 0.5, we use the sparse data
        % structure.
        if den <= 0.5
            save_problem(P, full_path, 1);
        else
            save_problem(P, full_path, 0); 
        end
    end
    
end