/*
    File:       Projection.hpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019

    This file contains the definition of several algorithms that can be used
    to project a point onto the feasible region of our optimization problem.
*/

#ifndef PROJECTION_H
#define PROJECTION_H

#define ARMA_DONT_USE_WRAPPER
#include <armadillo>
#include "Problem.hpp"

// Checks if a vector x is on the unit simplex.
bool on_unit_simplex(const arma::vec &x, double eps = ABS_TOL);

// Projection of point x onto the unit simplex using sorting.
arma::vec sorting_unit_simplex_proj(const arma::vec &x);

// Projects a point x onto the feasible region of the problem P.
arma::vec project(QProblem &P, const arma::vec &x);

#endif
