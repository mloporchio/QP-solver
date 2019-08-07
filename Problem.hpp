/*
    File:       Problem.hpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#ifndef PROBLEM_H
#define PROBLEM_H

#define ARMA_DONT_USE_WRAPPER
#include <armadillo>
#include <optional>
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

    // This method checks if a point is in the feasible region of the problem.
    bool isFeasible(const arma::vec &x, double eps) {
        return (arma::approx_equal(A * x, b, "absdiff", eps) &&
        arma::all(x >= -eps));
    }

    // Returns a feasible starting point where each component
    // of a partition S is equal to 1 / # of elements in S.
    arma::vec initial_point() {
        arma::vec result = arma::zeros(q.n_elem);
        A.each_row([&](arma::rowvec &v) {
            arma::uvec idx = arma::find(v > 0);
            result(idx).fill(1.0 / ((double) idx.n_elem));
        });
        return result;
    }
};

// This struct contains the output of the PGM.
struct QResult {
    arma::vec x; // Solution.
    double v; // Optimal value.
    arma::uword n_iter; // Total number of iterations of the PGM.
};

// Computes the maximum feasible stepsize.
double max_step(const arma::vec &x, const arma::vec &d, const arma::uvec &act,
double eps);

// This is the implementation of the projected gradient method.
QResult PGM(QProblem &P, const arma::vec &x_0, arma::uword max_iter,
double tol_cnst, double tol_opt);

#endif
