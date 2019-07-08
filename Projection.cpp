/*
    File:       Projection.cpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#include <cmath>
#include "Projection.hpp"

// Checks if a vector x is on the unit simplex.
bool on_unit_simplex(const arma::vec &x, double eps) {
    return ((abs(arma::accu(x) - 1) < eps) && arma::all(x >= 0));
}

// Projection of point x onto the unit simplex using sorting.
arma::vec sorting_unit_simplex_proj(const arma::vec &x) {
    // Check if x is already on the simplex.
    if (on_unit_simplex(x)) return x;
    arma::uword k = 0;
    arma::vec u = arma::sort(x, "descend");
    arma::vec c = arma::cumsum(u);
    for (arma::uword i = 0; i < u.n_elem; i++) {
        if (u(i) > ((c(i)-1)/(i+1))) k = i+1;
    }
    double t = (c(k-1)-1)/k;
    return arma::max(arma::zeros(x.n_elem), x - t);
}

// Projects a point x onto the feasible region of the problem P.
arma::vec project(QProblem &P, const arma::vec &x) {
    arma::vec result = arma::zeros(x.n_elem);
    P.A.each_row([&](arma::rowvec &v) {
        arma::uvec idx = arma::find(v > 0);
        result(idx) = sorting_unit_simplex_proj(x(idx));
    });
    return result;
}
