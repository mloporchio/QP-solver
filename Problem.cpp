/*
    File:       Problem.cpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#include "Problem.hpp"
#include "Projection.hpp"

// Performs a line search to find the step size.
double lineSearch(QProblem &P, double alpha_0, double tau, double beta,
const arma::vec &x, const arma::vec &g, const arma::vec &d) {
    double alpha = alpha_0;
    double z = arma::dot(g, d);
    while (P.f(x + alpha * d) > P.f(x) + alpha * beta * z) alpha *= tau;
    return alpha;
}

// Implements the stopping criterion of the PGM.
bool stopping(const arma::vec &x, const arma::vec &d, double atol,
double rtol) {
    return arma::approx_equal(x, d, "both", atol, rtol);
}

// This is the implementation of the projected gradient method.
QResult PGM(QProblem &P, const arma::vec &x_0, double alpha_0, double tau,
double beta, unsigned int max_iter, double atol, double rtol) {
    unsigned int k = 0;
    arma::vec x = x_0;
    while (k < max_iter) {
        double value = P.f(x);
        arma::vec g = P.gf(x);
        arma::vec d = project_block(P, x - g);
        if (stopping(x, d, atol, rtol)) break;
        else d = d - x;
        double alpha = lineSearch(P, alpha_0, tau, beta, x, g, d);
        x = x + alpha * d;
        k++;
    }
    return {x, P.f(x), k};
}
