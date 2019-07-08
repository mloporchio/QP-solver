/*
    File:       Initial.cpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#include "Initial.hpp"

// Returns a random point inside the feasible region of the problem.
arma::vec initial_point_first(QProblem &P) {
    arma::vec result = arma::zeros(P.q.n_elem);
    P.A.each_row([&](arma::rowvec &v) {
        arma::uvec idx = arma::find(v, 1, "first");
        result(idx(0)) = 1;
    });
    return result;
}

// Returns a random point inside the feasible region of the problem.
arma::vec initial_point_equal(QProblem &P) {
    arma::vec result = arma::zeros(P.q.n_elem);
    P.A.each_row([&](arma::rowvec &v) {
        arma::uvec idx = arma::find(v > 0);
        // Each component of a partition S is 1 / # of elements in S.
        result(idx).fill(1.0 / ((double) idx.n_elem));
    });
    return result;
}
