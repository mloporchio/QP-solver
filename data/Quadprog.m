%   
%   File:   quadprog.m
%   Author: Matteo Loporchio
%
%   In this file we try to solve our problem using Matlab's 
%   quadprog solver. The solutions can be used as a benchmark
%   to assess the quality of our C++ implementation.
%   

Q = csvread('data_Q.csv', 0, 0);
A = csvread('data_A.csv', 0, 0);
u = csvread('data_u.csv', 0, 0);
b = ones(size(A, 1), 1);

quadprog(2 * Q, u, [], [], A, b, [], [])