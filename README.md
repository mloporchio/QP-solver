# QP-solver

A convex quadratic program solver written in C++.
The tool is based on the gradient projection method and aims to solve the following optimization problem:

  minimize f(x) = x^T * Q * x + u^T * x
  s.t. Ax = b
  
where Q is a n x n positive semidefinite matrix.
