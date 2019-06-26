/*
    File:       Problem.hpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#ifndef PROBLEM_H
#define PROBLEM_H

#define ARMA_DONT_USE_WRAPPER
#include <armadillo>
#include "Config.hpp"

/*
    This struct represents our quadratic problem:

        \min (x^T * Q * x + u^T * x)
        s.t. \sum_{i in I^h}{x_i} = 1 for h = 1,...,k and x in R^n

    where the index sets I_h with h = 1,...,k form a partition of the set
    {1,...,n} and Q is positive semidefinite.
*/
struct QProblem {
    arma::mat Q;
    arma::vec q;
    arma::mat A; // k * n matrix of elements in {0, 1}
    arma::vec b; // k * 1 vector of ones.

    // Objective function.
    double f(const arma::vec &x) {
        return arma::dot(x, Q * x) + arma::dot(q, x);
    }

    // Gradient of the objective function.
    arma::vec gf(const arma::vec &x) {
        return 2 * (Q * x) + q;
    }

    // Check if a vector falls in the feasible region of the problem.
    bool isFeasible(const arma::vec &x, double atol = ABS_TOL,
    double rtol = REL_TOL) {
        return (arma::approx_equal(A * x, b, "both", atol, rtol) &&
        arma::all(x >= 0));
    }

    // Returns a random point inside the feasible region of the problem.
    arma::vec initialPoint() {
        arma::vec result = arma::zeros(q.n_elem);
        A.each_row([&](arma::rowvec &v) {
            arma::uvec idx = arma::find(v, 1, "first");
            result(idx(0)) = 1;
        });
        return result;
    }
};

// This struct contains the output of the PGM.
struct QResult {
    arma::vec x; // Solution.
    double v; // Optimal value.
    unsigned int n_iter; // Total number of iterations of the PGM.
};

// Performs a line search to find the step size.
double lineSearch(QProblem &P, double alpha_0, double tau, double beta,
const arma::vec &x, const arma::vec &g, const arma::vec &d);

// Implements the stopping criterion of the PGM.
bool stopping(const arma::vec &x, const arma::vec &d,
double atol = ABS_TOL, double rtol = REL_TOL);

// This is the main implementation of the projected gradient method.
QResult PGM(QProblem &P, const arma::vec &x_0, double alpha_0, double tau,
double beta, unsigned int max_iter, double atol = ABS_TOL,
double rtol = REL_TOL);

#endif
