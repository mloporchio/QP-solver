%
%   File:       generate_problem.m
%   Authors:    Matteo Loporchio, Davide Rucci
%
%   This function can be used to generate a problem.
%   
%   

function P = generate_problem(n, k, ecc)
    rank = 1.1;
    % Generate a random matrix.
    G = randn(round(rank * n), n);
    Q = G' * G;
    % Compute the eigenvalue decomposition of Q.
    [V, D] = eig(Q);
    % Modify the eccentricity of Q.
    if D(1, 1) > 1e-14
        d = diag(D);
        l = d(1) * ones(n, 1) + (d(1) / (d(n) - d(1))) * ...
            (2 * ecc / (1 - ecc)) * (d - d(1) * ones(n, 1));  
        Q = V * diag(l) * V';
    end
    % Generate a random vector q.
    q = randn(n, 1);
    % Generate a random constraint matrix.
    A = random_constr(n, k);
    % Build the full struct.
    P = struct('Q', Q, 'q', q, 'A', A);
end