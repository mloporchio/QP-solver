/*
    File:       Projection.cpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#include "Projection.hpp"

// Computes the projection of the gradient onto the subspace A_bar * d = 0.
arma::vec project_block(QProblem &P, const arma::uvec &act,
const arma::vec &g) {
    arma::vec d = arma::zeros(g.n_elem);
    // For each simplex...
    for (size_t i = 0; i < P.c.size(); i++) {
        // Extract the corresponding block.
        arma::uvec idx = P.c.at(i);
        // Project onto the single block.
        d(idx) = sub_proj(g(idx), act(idx));
    }
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
