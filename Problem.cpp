/*
    File:       Problem.cpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#include <cmath>
#include "Problem.hpp"
#include "Projection.hpp"
#include "Utils.hpp"

// Computes the currently active constraints.
void set_active(const arma::vec &x, arma::uvec &act, double eps) {
    for (arma::uword i = 0; i < x.n_elem; i++) {
        act(i) = ((x(i) <= eps) ? 1 : 0);
    }
}

// Computes the maximum feasible stepsize.
double max_step(const arma::vec &x, const arma::vec &d, const arma::uvec &act,
double eps) {
    double step = arma::datum::inf;
    for (arma::uword i = 0; i < x.n_elem; i++) {
        if (!act(i) && -d(i) > eps) step = fmin(step, -x(i)/d(i));
    }
    return step;
}

// This is the implementation of the projected gradient method.
QResult PGM(QProblem &P, const arma::vec &x_0, arma::uword max_iter,
double ctol, double dtol) {
    arma::uword k = 0;
    arma::vec x = x_0, g = P.gf(x);
    arma::uvec act(x.n_elem, arma::fill::zeros);
    while (k < max_iter) {
        // Compute the active inequality constraints.
        set_active(x, act, ctol);
        // Compute the projected direction.
        arma::vec d = project_block(P, act, g);
        // Stop if the norm of the direction is (nearly) zero.
        if (arma::norm(d) <= dtol) break;
        // Compute the maximum step to the nearest intersecting boundary.
        double alpha_u = max_step(x, d, act, ctol);
        // Then perform the line search.
        arma::vec p = 2 * P.Q * d;
        double alpha = fmin(alpha_u, -arma::dot(g, d) / arma::dot(d, p));
        // Move to the next point.
        x = x + alpha * d;
        // Update the gradient.
        g = g + alpha * p;
        k++;
    }
    return {x, P.f(x), k};
}
