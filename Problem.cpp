/*
    File:       Problem.cpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#include <cmath>
#include "Problem.hpp"
#include "Projection.hpp"

// Performs a line search to find the step size.
double line_search(QProblem &P, const arma::vec &g, const arma::vec &d,
double atol, double rtol) {
    double max_step = 1, den = arma::dot(d, P.Q * d);
    if (abs(den) <= atol) return max_step;
    return fmin(max_step, arma::dot(-g, d) / den);
}

// Implements the stopping criterion of the PGM.
bool stopping(const arma::vec &x, const arma::vec &d, double atol,
double rtol) {
    return arma::approx_equal(x, d, "both", atol, rtol);
}

// This is the implementation of the projected gradient method.
QResult PGM(QProblem &P, const arma::vec &x_0, unsigned int max_iter,
double s, double atol, double rtol) {
    unsigned int k = 0;
    arma::vec x = x_0;
    while (k < max_iter) {
        arma::vec g = P.gf(x);
        arma::vec x_bar = project(P, x - s * g);
        if (stopping(x, x_bar, atol, rtol)) break;
        arma::vec d = x_bar - x;
        double alpha = line_search(P, g, d, atol, rtol);
        x = x + alpha * d;
        k++;
    }
    return {x, P.f(x), k};
}
