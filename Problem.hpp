/*
    File:       Problem.hpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#ifndef PROBLEM_H
#define PROBLEM_H

#define ARMA_DONT_USE_WRAPPER
#include <armadillo>
#include <cmath>
#include "Config.hpp"

/*
    The constraints of the problem are represented as a list of vectors.
    Each vector contains the indices of the non-zero components of the
    gradient of the constraint.
*/
typedef std::vector<arma::uvec> cstlist_t;

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
    cstlist_t c;

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
        for (size_t i = 0; i < c.size(); i++) {
            arma::uvec idx = c.at(i);
            if (fabs(arma::accu(x(idx)) - 1) > eps) return false;
        }
        return arma::all(x >= -eps);
    }

    // Returns a feasible starting point where each component
    // of a partition S is equal to 1 / # of elements in S.
    arma::vec initial_point() {
        arma::vec result = arma::zeros(q.n_elem);
        for (size_t i = 0; i < c.size(); i++) {
            arma::uvec idx = c.at(i);
            result(idx).fill(1.0 / ((double) idx.n_elem));
        }
        return result;
    }
};

// This struct contains the output of the PGM.
struct QResult {
    arma::vec x; // Solution.
    double v; // Optimal value.
    arma::uword n_iter; // Total number of iterations of the PGM.
};

// Computes the currently active constraints.
void set_active(const arma::vec &x, arma::uvec &act, double eps);

// Computes the maximum feasible stepsize.
double max_step(const arma::vec &x, const arma::vec &d, const arma::uvec &act,
double eps);

// This is the implementation of the projected gradient method.
QResult PGM(QProblem &P, const arma::vec &x_0, arma::uword max_iter,
double ctol, double dtol);

#endif
