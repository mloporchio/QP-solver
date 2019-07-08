/*
    File:       Initial.hpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#include "Problem.hpp"

// Returns a random point inside the feasible region of the problem.
arma::vec initial_point_first(QProblem &P);

// Returns a random point inside the feasible region of the problem.
arma::vec initial_point_equal(QProblem &P);

// Returns a random point inside the feasible region of the problem.
arma::vec initial_point_random(QProblem &P);
