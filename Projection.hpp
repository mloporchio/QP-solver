/*
    File:       Projection.hpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#ifndef PROJECTION_H
#define PROJECTION_H

#include "Problem.hpp"

// Computes the projection of a vector onto the subspace A_bar * d = 0.
// This method uses Armadillo's solver for systems of linear equations.
arma::vec project_auto(const arma::mat &A_bar, const arma::vec &x);

// Computes the projection of a vector onto the subspace A_bar * d = 0.
// This method explicitly computes the projection matrix.
arma::vec project_mat(const arma::mat &A_bar, const arma::vec &x);

// Computes the projection of a vector onto the subspace A_bar * d = 0.
arma::vec project_block(QProblem &P, const arma::uvec &act, const arma::vec &g);

// Projects the gradient onto the single block of coordinates.
arma::vec sub_proj(const arma::vec &g, const arma::uvec &act);

#endif
