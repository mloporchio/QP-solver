/*
    File:       Problem.cpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#ifndef PROBLEM_IMPL_
#define PROBLEM_IMPL_

#include "Problem.hpp"
#include "Config.hpp"

// Computes the currently active constraints.
template<typename T>
void QProblem<T>::set_active(const arma::vec &x, arma::uvec &act, double eps) {
    for (arma::uword i = 0; i < x.n_elem; i++) act(i) = (x(i) <= eps);
}

// Computes the maximum feasible stepsize.
template<typename T>
double QProblem<T>::max_step(const arma::vec &x, const arma::vec &d,
const arma::uvec &act, double eps) {
    double step = arma::datum::inf;
    for (arma::uword i = 0; i < x.n_elem; i++) {
        if (!act(i) && -d(i) > eps) step = fmin(step, -x(i)/d(i));
    }
    return step;
}

// Performs a line search in the interval [0, alpha_bar].
template<typename T>
double QProblem<T>::line_search(const arma::vec &g, const arma::vec &d,
const arma::vec &l, double alpha_bar, double eps) {
    double den = arma::dot(d, l);
    if (den <= eps) return alpha_bar;
    return fmin(alpha_bar, -arma::dot(g, d) / den);
}

// Computes the projection of the gradient onto the subspace A_bar * d = 0.
template<typename T>
arma::vec QProblem<T>::project(const arma::uvec &act, const arma::vec &g) {
    arma::vec d = arma::zeros(g.n_elem);
    // For each simplex...
    for (size_t i = 0; i < c.size(); i++) {
        // Extract the corresponding block.
        arma::uvec idx = c.at(i);
        // Project onto the single block.
        d(idx) = sub_proj(g(idx), act(idx));
    }
    return d;
}

// Projects the gradient onto the single block of coordinates.
template<typename T>
arma::vec QProblem<T>::sub_proj(const arma::vec &g, const arma::uvec &act) {
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

template<typename T>
QProblem<T>::QProblem(T &Q, arma::vec &q, cstlist_t &c) {
    this -> Q = Q;
    this -> q = q;
    this -> c = c;
}

// This is the objective function.
template<typename T>
double QProblem<T>::f(const arma::vec &x) {
    return arma::dot(x, Q * x) + arma::dot(q, x);
}

// This is the gradient of the objective function.
template<typename T>
arma::vec QProblem<T>::gf(const arma::vec &x) {
    return 2 * (Q * x) + q;
}

// This method checks if a point is in the feasible region of the problem.
template<typename T>
bool QProblem<T>::is_feasible(const arma::vec &x, double eps) {
    for (size_t i = 0; i < c.size(); i++) {
        arma::uvec idx = c.at(i);
        if (fabs(arma::accu(x(idx)) - 1) > eps) return false;
    }
    return arma::all(x >= -eps);
}

/*
    Returns a feasible starting point where each component
    of a partition S is equal to 1 / # of elements in S.
*/
template<typename T>
arma::vec QProblem<T>::initial_point() {
    arma::vec result = arma::zeros(q.n_elem);
    for (size_t i = 0; i < c.size(); i++) {
        arma::uvec idx = c.at(i);
        result(idx).fill(1.0 / ((double) idx.n_elem));
    }
    return result;
}

/*
    This is the implementation of the projected gradient method.
    The following input parameters are required:

        - x_0, the initial feasible point.
        - max_iter, a limit on the number of iterations.
        - ctol, constraint tolerance.
        - dtol, norm tolerance.
*/
template<typename T>
QResult QProblem<T>::PGM(const arma::vec &x_0, arma::uword max_iter,
double ctol, double dtol) {
    auto t1 = std::chrono::high_resolution_clock::now();
    arma::uword k = 0;
    arma::vec x = x_0, g = gf(x);
    arma::uvec act(x.n_elem, arma::fill::zeros);
    std::vector<double> hist;
    while (k < max_iter) {
        #if LOG_VAL
        hist.push_back(f(x));
        #endif
        // Compute the active inequality constraints.
        set_active(x, act, ctol);
        // Compute the projected direction.
        arma::vec d = project(act, g);
        // Stop if the norm of the direction is (nearly) zero.
        if (arma::norm(d) <= dtol) break;
        // Compute the maximum step to the nearest intersecting boundary.
        double alpha_bar = max_step(x, d, act, dtol);
        // Then perform the line search.
        arma::vec l = 2 * Q * d;
        double alpha = line_search(g, d, l, alpha_bar);
        // Move to the next point and update the gradient.
        x = x + alpha * d;
        g = g + alpha * l;
        k++;
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    unsigned long t = std::chrono::duration_cast
    <std::chrono::microseconds>(t2 - t1).count();
    return {x, f(x), k, hist, t};
}

#endif
