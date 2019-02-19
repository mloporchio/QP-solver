/*
 *  File: main.cpp
 *  Authors: Matteo Loporchio, Davide Rucci
 * 
 *  Computational Mathematics for Learning and Data Analysis, A.Y. 2018-2019
 */

#define ARMA_DONT_USE_WRAPPER
#include <armadillo>
#include <iostream>

typedef double (*function_ptr)(arma::mat, arma::vec, arma::vec);
typedef arma::vec (*gradient_ptr)(arma::mat, arma::vec, arma::vec);

// Objective function.
double f(arma::mat Q, arma::vec q, arma::vec x) {
    return arma::dot(x, Q * x) + arma::dot(q, x);
}

// Gradient of the objective function.
arma::vec gf(arma::mat Q, arma::vec q, arma::vec x) {
    return 2 * (Q * x) + q;
}

// This data structure represents the following quadratic problem:
//
//      \min (x^T * Q * x + q^T * x)
//      s.t. \sum_{i \in I^h}{x_i} = 1 for h = 1,...,k and x \in R^n
//
// where the index sets I_h with h = 1,...,k form a partition of the set 
// {1,...,n} and Q is positive semidefinite.
typedef struct {
    arma::mat Q;
    arma::vec q; 
    arma::mat A; // k * n matrix of elements in {0, 1}
    arma::vec b; // k * 1 vector of ones.
    function_ptr f;
    gradient_ptr gf;
} problem_t;

#if 0
// Generates a random n x n p.d. symmetrix matrix.
// https://math.stackexchange.com/questions/357980/how-to-generate-random-symmetric-positive-definite-matrices-using-matlab
arma::mat generate_matrix(arma::uword n) {
    arma::mat M(n, n, arma::fill::randn);
    M = 0.5 * (M + M.t());
    M = M + n * arma::eye(n, n);
    return M;
}
#endif

// Solves the direction-finding problem at each step of the PGM.
arma::vec solve_DFP(const arma::mat &A, const arma::vec &grad) {
    arma::uword k = A.n_rows, n = A.n_cols;
    // Build the matrix U.
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
    arma::vec d(n);
    for (arma::uword i = 0; i < n; i++) d(i) = y(i); 
    return d;
}

// Performs line search to find the step size.
double armijo_back_ls(problem_t p, const arma::vec &x, const arma::vec &g,
const arma::vec &d, double alpha_init, double tau, double beta) {
    double alpha = alpha_init, v1, v2;
    do {
        v1 = p.f(p.Q, p.q, x + alpha * d);
        v2 = p.f(p.Q, p.q, x) + alpha * beta * arma::dot(g, d);
        alpha = tau * alpha;
    } while (v1 > v2);
    return alpha;
}

//
arma::vec PGM(problem_t p, double tol, int max_iter, double alpha_init, 
double tau, double beta) {
    // Initialize x with a point in the feasible region.
    arma::vec x = arma::solve(p.A, p.b);
    int k = 0;
    while (k < max_iter) {
        // Compute the value of the function.
        double v = p.f(p.Q, p.q, x);
        // Compute the gradient.
        arma::vec g = p.gf(p.Q, p.q, x);
        // Solve the DFP.
        arma::vec d = solve_DFP(p.A, g);
        // if grad(f(x_k))^T * d_k = 0 then stop. x_k is a KKT point.
        double t = arma::dot(g, d);
        if (t == 0) break;
        // Choose step size a by performing an exact (or inexact) line search.
        double alpha = armijo_back_ls(p, x, g, d, alpha_init, tau, beta);
        // Update x.
        x = x + alpha * d;
        k++;
    }
    std::cout << "Converged at iteration = " << k << std::endl;
    return x;
}

int main(int argc, char const *argv[]) {
    arma::mat Q(7, 7, arma::fill::randn);
    arma::vec q(7, arma::fill::randn);
    arma::mat A;
    A << 0 << 1 << 0 << 0 << 1 << 1 << 0 << arma::endr
    << 1 << 0 << 1 << 0 << 0 << 0 << 0 << arma::endr
    << 0 << 0 << 0 << 1 << 0 << 0 << 1 << arma::endr;
    arma::vec b(3, arma::fill::ones);
    problem_t r = {.Q = Q, .q = q, .A = A, .b = b, .f = f, .gf = gf};
    arma::vec z = PGM(r, 10E-8, 10000, 1, 0.01, 0.01);
    std::cout << "Solution = " << z.t();
    std::cout << "Az = " << (A * z).t();
    return 0;
}
