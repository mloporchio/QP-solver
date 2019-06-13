/*
    File:       Problem.cpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#include "Problem.hpp"

// Loads problem data from a set of CSV files.
QProblem load(std::string path, std::string name) {
    // Build the names.
    std::stringstream name1, name2, name3;
    name1 << path << "/" << name << "_Q.csv";
    name2 << path << "/" << name << "_A.csv";
    name3 << path << "/" << name << "_u.csv";
    // Load Q, A and q from the CSV files.
    arma::mat Q, A;
    arma::vec q;
    Q.load(name1.str(), arma::csv_ascii);
    A.load(name2.str(), arma::csv_ascii);
    q.load(name3.str(), arma::csv_ascii);
    // Build the vector b.
    arma::vec b(A.n_rows, arma::fill::ones);
    QProblem p = {Q, q, A, b};
    return p;
}

// Check if a vector falls in the feasible region of the problem.
bool isFeasible(const arma::mat &A, const arma::vec &b, const arma::vec &x,
double tol) {
    return arma::approx_equal(A * x, b, "absdiff", tol);
}

// Returns true if x is close to zero with a given tolerance.
// Many thanks to:
// https://stackoverflow.com/questions/6982217/how-do-i-check-and-handle-numbers-very-close-to-zero
bool isCloseToZero(double x, double tol) {
    return std::abs(x) < tol; //std::numeric_limits<double>::epsilon();
}

// Solves the direction-finding problem at each step of the PGM.
arma::vec solveDFP(const arma::mat &A, const arma::vec &grad) {
    arma::uword k = A.n_rows, n = A.n_cols;
    // Build the matrix U.
    //
    //      | I  A^T  |
    //  U = |         |
    //      | A   0   |
    //
    arma::mat I = arma::eye(n, n);
    arma::mat X = arma::join_rows(I, A.t());
    arma::mat Y = arma::join_rows(A, arma::zeros(k, k));
    arma::mat U = arma::join_cols(X, Y);
    // Build the vector v.
    arma::vec v(n+k, arma::fill::zeros);
    for (arma::uword i = 0; i < grad.n_elem; i++) v(i) = -grad(i);
    // Solve the system Uy = v.
    arma::vec y = arma::solve(U, v);
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
arma::vec PGM(
    QProblem p,
    int maxIter,
    double alphaInit,
    double tau,
    double beta,
    double tol
){
    // Initialize x with a point in the feasible region.
    arma::vec x = arma::solve(p.A, p.b);
    int k = 0;
    while (k < maxIter) {
        // Compute the value of the function.
        double v = p.f(x);
        //std::cout << "v = " << v << std::endl;
        // Compute the gradient.
        arma::vec g = p.gf(x);
        // Solve the DFP.
        arma::vec d = solveDFP(p.A, g);
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
    std::cout << "Converged at iteration = " << k << std::endl;
    return x;
}
