%
%   File:       test_k.m
%   Authors:    Matteo Loporchio, Davide Rucci
%
%   This function can be used to generate 100 test cases with
%   n = 1000 and a variable number of simplices k.
%
function test_k(output_path)
    ntest = 100;
    for i = 1:ntest
        k = i * 10;
        P = generate_problem(1000, k, 0.5);
        name = sprintf('test_%d_%d', 1000, k);
        full_path = sprintf('%s/%s', output_path, name);
        % Save the data set to disk.
        save_problem(P, full_path, 0);
    end
end