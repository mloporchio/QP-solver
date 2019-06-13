/*
    File:       Problem.hpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#ifndef PROBLEM_H
#define PROBLEM_H

#define ARMA_DONT_USE_WRAPPER
#include <armadillo>

/*
    This struct represents our main quadratic problem:

        \min (x^T * Q * x + q^T * x)
        s.t. \sum_{i in I^h}{x_i} = 1 for h = 1,...,k and x in R^n

    where the index sets I_h with h = 1,...,k form a partition of the set
    {1,...,n} and Q is positive semidefinite.
*/
struct QProblem {
    arma::mat Q;
    arma::vec q;
    arma::mat A; // k * n matrix of elements in {0, 1}
    arma::vec b; // k * 1 vector of ones.
    // Definition of the objective function.
    double f(const arma::vec &x) {
        return arma::dot(x, Q * x) + arma::dot(q, x);
    }
    // Definition of the gradient of the objective function.
    arma::vec gf(const arma::vec &x) {
        return 2 * (Q * x) + q;
    }
};

// Creates a new problem by loading its data from a set of files.
QProblem load(std::string path, std::string name);

// Check if a vector falls in the feasible region of the problem.
bool isFeasible(const arma::mat &A, const arma::vec &b, const arma::vec &x,
double tol);

// Returns true if x is close to zero with a given tolerance.
bool isCloseToZero(double x, double tol);

// Solves the direction-finding problem at each step of the PGM.
arma::vec solveDFP(const arma::mat &A, const arma::vec &grad);

// Performs a line search to find the step size.
// The currently implemented strategy is Armijo's with backtracking.
double lineSearch(QProblem p, const arma::vec &x, const arma::vec &g,
const arma::vec &d, double alphaInit, double tau, double beta);

// Main implementation of the projected gradient method.
arma::vec PGM(QProblem p, int maxIter, double alphaInit, double tau,
double beta, double tol);

#endif
