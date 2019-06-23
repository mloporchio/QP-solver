%   
%   File:   Quadprog.m
%   Author: Matteo Loporchio
%
%   In this file we try to solve our problem using Matlab's 
%   quadprog solver. The solutions can be used as a benchmark
%   to assess the quality of our C++ implementation.
%   

Q = csvread('test2_Q.csv', 0, 0);
A = csvread('test2_A.csv', 0, 0);
u = csvread('test2_u.csv', 0, 0);
b = ones(size(A, 1), 1);
z = zeros(size(Q, 1), 1);

[x, v] = quadprog(2 * Q, u, [], [], A, b, z, []);

%f = [1, 1, 1];
%linprog(f,[],[],A,b,z,[])