/*
 *  File: main.cpp
 *  Authors: Matteo Loporchio, Davide Rucci
 * 
 *  Computational Mathematics for Learning and Data Analysis, A.Y. 2018-2019
 */

#define ARMA_DONT_USE_WRAPPER
#include <armadillo>
#include <iostream>

//
typedef double (*objfun_ptr)(arma::vec);
// 
typedef arma::vec (*gradient_ptr)(arma::vec);

// This data structure represents the following quadratic problem:
//
//      \min (x^T * Q * x + q^T * x)
//      s.t. \sum_{i \in I^h}{x_i} = 1 for h = 1,...,k and x \in R^n
//
// where the index sets I_h with h = 1,...,k form a partition of the set 
// {1,...,n} and Q is positive semidefinite.
typedef struct {
    objfun_ptr f;
    gradient_ptr grad_f;
    arma::mat Q;
    arma::vec q; 
    arma::mat A; // k * n matrix of elements in {0, 1}
    arma::vec b; // k * 1 vector of ones.
} quad_problem_t;

// Solves the direction-finding problem at each step of the PGM.
arma::vec solve_DFP(const arma::mat &A, const arma::vec &grad) {
    arma::uword k = A.n_rows, n = A.n_cols;
    // Build the matrix U.
    arma::mat X = arma::join_rows(arma::eye(n, n), A.t());
    arma::mat Y = arma::join_rows(A, arma::zeros(k, k));
    arma::mat U = arma::join_cols(X, Y);
    // Build the vector v.
    arma::vec v(n+k, arma::fill::zeros);
    for (arma::uword i = 0; i < grad.n_elem; i++) v(i) = grad(i); 
    // Solve the system Uy = v.
    arma::vec y = arma::solve(U, v);
    // Build the final result.
    arma::vec d(n);
    for (arma::uword i = 0; i < grad.n_elem; i++) d(i) = y(i); 
    return d;
}

// Performs line search to find the step size.
double backtracking_line_search(objfun_ptr f, const arma::vec &x, 
const arma::vec &d, double alpha_init, double tau) {
    double alpha = alpha_init;
    while (f(x + alpha * d) >= f(x)) alpha = tau * alpha;
    return alpha;
}

//f(x) = x.t() * qp.Q * x + qp.q.t() * x;
//Q * x + qp.q;

//
arma::vec PGM(quad_problem_t qp, double tol, int max_iter) {
    // Initialize x.
    arma::vec x; // Initial point (must satisfy Ax = b. How to choose?).
    int k = 0;
    while (k < max_iter) {
        // Compute the value of the function.
        double v = qp.f(x);
        // Compute the gradient.
        arma::vec g = qp.grad_f(x);
        // Solve the DFP.
        arma::vec d = solve_DFP(qp.A, g);
        // if grad(f(x_k))^T * d_k = 0 then stop. x_k is a KKT point.
        if (arma::norm(g.t() * d) <= tol) return x;
        // Choose step size a by performing an exact (or inexact) line search.
        double alpha = backtracking_line_search(qp.f, x, d, 1, 0.5);
        // Update x.
        x = x + alpha * d;
        k++;
    }
    return x;
}

int main(int argc, char const *argv[]) {
    arma::mat A(10, 20, arma::fill::randn);
    arma::vec g(20, arma::fill::randn); 
    arma::vec s = solve_DFP(A, g);
    std::cout << s;
    return 0;
}
