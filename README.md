# QP-solver

A convex quadratic program solver written in C++ and based on Rosen's <strong>projected gradient method</strong>.

This tool aims to solve the following optimization problem
whose feasible region is defined in terms of <a href="https://www.codecogs.com/eqnedit.php?latex=k" target="_blank"><img src="https://latex.codecogs.com/gif.latex?k" title="k" /></a> disjoint simplices in <a href="https://www.codecogs.com/eqnedit.php?latex=\mathbf{R}^n" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\mathbf{R}^n" title="\mathbf{R}^n" /></a>:

<a href="https://www.codecogs.com/eqnedit.php?latex=\begin{cases}&space;\min&space;{x^T&space;Q&space;x&space;&plus;&space;q^T&space;x}\\&space;\sum_{i&space;\in&space;I^h}{x_i}&space;=&space;1&space;&&space;\text{for}\&space;h&space;=&space;1,\dotsc,k\\&space;x&space;\in&space;\mathbf{R}^n\\&space;\end{cases}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\begin{cases}&space;\min&space;{x^T&space;Q&space;x&space;&plus;&space;q^T&space;x}\\&space;\sum_{i&space;\in&space;I^h}{x_i}&space;=&space;1&space;&&space;\text{for}\&space;h&space;=&space;1,\dotsc,k\\&space;x&space;\geq&space;0\\&space;\end{cases}" title="problem" /></a>

where <img src="https://latex.codecogs.com/gif.latex?Q" title="Q" /> is a <a href="https://www.codecogs.com/eqnedit.php?latex=n&space;\times&space;n" target="_blank"><img src="https://latex.codecogs.com/gif.latex?n&space;\times&space;n" title="n \times n" /></a> positive semidefinite matrix.
