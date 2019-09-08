%
%   File:       random_constr.m
%   Authors:    Matteo Loporchio, Davide Rucci
%
%   This function can be used to generate a random constraint matrix
%   with full row rank.

function M = random_constr(n, k)
    % Initialize the result.
    M = zeros(k, n);
    % Randomly shuffle the range of indices [1, n].
    idx = (1:n)';
    idx = idx(randperm(length(idx)));
    % Generate a vector of k positive integers that sum up to n.
    % These integers represent the sizes of the partitions.
    size = random_partition(k, n);
    % For each partition, let s be its size.
    % We have to read s elements from the shuffled indices and then
    % add the corresponding constraints to the result.
    low = 0;
    for i = 1:k
        l = zeros(size(i), 1);
        for j = 1:size(i)
            l(j) = idx(low + j);
        end
        low = low + size(i);
        l = sort(l);
        for j = 1:size(i)
            M(i, l(j)) = 1; 
        end
    end
end

% This function can be used to generate a random partitioning of
% the range [1, n] into k disjoint sets.
function p = random_partition(k, n)
    p = zeros(k, 1);
    ind = zeros(k+1, 1);
    ind(1) = 0;
    ind(k+1) = n;
    y = sort(randsample(n - 1, k - 1));
    for i = 2:k
        ind(i) = y(i-1);
    end
    for i = 1:k
        p(i) = ind(i+1) - ind(i);
    end
end