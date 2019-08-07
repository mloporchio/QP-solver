/*
    File:       Projection.cpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#include "Projection.hpp"

// Computes the projection of a vector onto the subspace A_bar * d = 0.
// This method uses Armadillo's solver for systems of linear equations.
arma::vec project_auto(const arma::mat &A_bar, const arma::vec &x) {
    arma::uword k = A_bar.n_rows, n = A_bar.n_cols;
    // Build the matrix U.
    //
    //      |   I    A_bar^T |
    //  U = |                |
    //      | A_bar     0    |
    //
    arma::mat I = arma::eye(n, n);
    arma::mat X = arma::join_rows(I, A_bar.t());
    arma::mat Y = arma::join_rows(A_bar, arma::zeros(k, k));
    arma::mat U = arma::join_cols(X, Y);
    // Build the vector v.
    arma::vec v(n + k, arma::fill::zeros);
    for (arma::uword i = 0; i < x.n_elem; i++) v(i) = x(i);
    // Solve the system Uy = v.
    arma::vec y = arma::solve(U, v);
    // Build the final result.
    return y.subvec(0, n - 1);
}

// Computes the projection of a vector onto the subspace A_bar * d = 0.
// This method explicitly computes the projection matrix.
arma::vec project_mat(const arma::mat &A_bar, const arma::vec &x) {
    arma::mat I = arma::eye(A_bar.n_cols, A_bar.n_cols);
    arma::mat P = I - (A_bar.t() * arma::inv(A_bar * A_bar.t()) * A_bar);
    return P * x;
}

// Computes the projection of the gradient onto the subspace A_bar * d = 0.
arma::vec project_block(QProblem &P, const arma::uvec &act,
const arma::vec &g) {
    arma::vec d = arma::zeros(g.n_elem);
    // For each row of A...
    P.A.each_row([&](arma::rowvec &v) {
        // Extract the block corresponding to the simplex.
        arma::uvec idx = arma::find(v > 0);
        // Project onto the single block.
        d(idx) = sub_proj(g(idx), act(idx));
    });
    return d;
}

// Projects the gradient onto the single block of coordinates.
arma::vec sub_proj(const arma::vec &g, const arma::uvec &act) {
    arma::uword n = g.n_elem;
    // Compute the number of active constraints.
    arma::uword nact = arma::accu(act);
    // Create the result vector.
    arma::vec d = arma::zeros(n);
    // Compute the mean over the non active constraints.
    double m = 0;
    for (arma::uword i = 0; i < d.n_elem; i++) if (!act(i)) m += g(i);
    m /= (double)(n - nact);
    // Set the components of the result.
    for (arma::uword i = 0; i < d.n_elem; i++) d(i) = ((act(i)) ? 0 : m - g(i));
    return d;
}
