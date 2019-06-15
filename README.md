# QP-solver

A convex quadratic program solver written in C++.

The tool is based on the gradient projection method and aims to solve the following optimization problem:

<a href="https://www.codecogs.com/eqnedit.php?latex=\begin{cases}&space;\min&space;{x^T&space;Q&space;x&space;&plus;&space;q^T&space;x}\\&space;\sum_{i&space;\in&space;I^h}{x_i}&space;=&space;1&space;&&space;\text{for}\&space;h&space;=&space;1,\dotsc,k\\&space;x&space;\in&space;\mathbf{R}^n\\&space;\end{cases}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\begin{cases}&space;\min&space;{x^T&space;Q&space;x&space;&plus;&space;q^T&space;x}\\&space;\sum_{i&space;\in&space;I^h}{x_i}&space;=&space;1&space;&&space;\text{for}\&space;h&space;=&space;1,\dotsc,k\\&space;x&space;\in&space;\mathbf{R}^n\\&space;\end{cases}" title="\begin{cases} \min {x^T Q x + q^T x}\\ \sum_{i \in I^h}{x_i} = 1 & \text{for}\ h = 1,\dotsc,k\\ x \in \mathbf{R}^n\\ \end{cases}" /></a>
  
where Q is a n x n positive semidefinite matrix.
