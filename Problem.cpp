/*
    File:       Problem.cpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#include "Problem.hpp"

// Check if a vector falls in the feasible region of the problem.
bool isFeasible(const arma::mat &A, const arma::vec &b, const arma::vec &x,
double tol) {
    return arma::approx_equal(A * x, b, "absdiff", tol);
}

// This function returns true if x is close to zero with a given tolerance.
// Many thanks to: https://stackoverflow.com/questions/6982217/how-do-i-check-and-handle-numbers-very-close-to-zero
bool isCloseToZero(double x, double tol) {
    return std::abs(x) < tol; //std::numeric_limits<double>::epsilon();
}

// Builds the matrix for the direction-finding problem.
arma::mat DFPMatrix(const arma::mat &Q, const arma::mat &A) {
    arma::uword k = A.n_rows, n = A.n_cols;
    return arma::join_cols(
        arma::join_rows(Q, A.t()),              //  Q   A^T
        arma::join_rows(A, arma::zeros(k, k))   //  A    0
    );
}

// Solves the direction-finding problem at each step of the PGM.
arma::vec solveDFP(const arma::mat &M, const arma::vec &grad){
    arma::uword n = grad.n_elem;
    // Build the vector v.
    arma::vec v(M.n_rows, arma::fill::zeros);
    for (arma::uword i = 0; i < n; i++) v(i) = -grad(i);
    // Solve the system My = v.
    arma::vec y = arma::solve(M, v);
    // Build the final result.
    return y.subvec(0, n-1);
}

// Performs a line search to find the step size.
// The currently implemented strategy is Armijo's with backtracking.
double lineSearch(
    QProblem p,
    const arma::vec &x,
    const arma::vec &g,
    const arma::vec &d,
    double alphaInit,
    double tau,
    double beta
){
    double alpha = alphaInit, v1, v2;
    do {
        v1 = p.f(x + alpha * d);
        v2 = p.f(x) + alpha * beta * arma::dot(g, d);
        alpha = tau * alpha;
    } while (v1 > v2);
    return alpha;
}

// Main implementation of the projected gradient method.
QResult PGM(
    QProblem p,
    unsigned int maxIter,
    double alphaInit,
    double tau,
    double beta,
    double tol
){
    unsigned int k = 0;
    std::vector<double> history;
    // Initialize x with a point in the feasible region.
    arma::vec x = arma::solve(p.A, p.b);
    // Build the DFP matrix.
    arma::mat M = DFPMatrix(p.Q, p.A);
    while (k < maxIter) {
        // Compute the value of the function.
        history.push_back(p.f(x));
        // Compute the gradient.
        arma::vec g = p.gf(x);
        // Solve the DFP.
        arma::vec d = solveDFP(M, g);
        // Compute the dot product g * d.
        // If it is zero, then stop because x is a KKT point.
        if (isCloseToZero(arma::dot(g, d), tol)) break;
        // Choose step size a by performing a line search.
        double alpha = lineSearch(p, x, g, d, alphaInit, tau, beta);
        // Update x.
        x = x + alpha * d;
        // Increment the iteration counter.
        k++;
    }
    return {x, k, history};
}
