/*
    File:       Problem.hpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#ifndef PROBLEM_H
#define PROBLEM_H

#define ARMA_DONT_USE_WRAPPER
#include <armadillo>
#include <chrono>
#include <cmath>

/*
    The constraints of the problem are represented as a list of vectors.
    Each vector contains the indices of the non-zero components of the
    gradient of the constraint.
*/
typedef std::vector<arma::uvec> cstlist_t;

/*
    This struct contains the results of the projected gradient algorithm.
*/
struct QResult {
    arma::vec x;                // Solution.
    double v;                   // Optimal value.
    arma::uword n_iter;         // Total number of iterations.
    std::vector<double> hist;   // History of the objective function values.
    unsigned long time;         // Execution time.
};

/*
    This is the main class of our project that represents
    the quadratic optimization problem. The class contains both
    the representation of the problem and the methods that can be used
    to solve it.
*/
template <class T>
struct QProblem {
    T Q;            // This is the Hessian matrix of the quadratic function.
    arma::vec q;    // This is the linear part.
    cstlist_t c;    // This is the list of constraints.

    // Computes the currently active constraints.
    void set_active(const arma::vec &x, arma::uvec &act, double eps) {
        for (arma::uword i = 0; i < x.n_elem; i++) act(i) = (x(i) <= eps);
    }

    // Computes the maximum feasible stepsize.
    double max_step(const arma::vec &x, const arma::vec &d,
    const arma::uvec &act, double eps) {
        double step = arma::datum::inf;
        for (arma::uword i = 0; i < x.n_elem; i++) {
            if (!act(i) && -d(i) > eps) step = fmin(step, -x(i)/d(i));
        }
        return step;
    }

    // Performs a line search in the interval [0, alpha_bar].
    double line_search(const arma::vec &g, const arma::vec &d,
    const arma::vec &l, double alpha_bar, double eps = 1E-16) {
        double den = arma::dot(d, l);
        if (den <= eps) return alpha_bar;
        return fmin(alpha_bar, -arma::dot(g, d) / den);
    }

    // Computes the projection of the gradient onto the subspace A_bar * d = 0.
    arma::vec project(const arma::uvec &act, const arma::vec &g) {
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
        for (arma::uword i = 0; i < d.n_elem; i++)
            d(i) = ((act(i)) ? 0 : m - g(i));
        return d;
    }

    // This is the objective function.
    double f(const arma::vec &x) {
        return arma::dot(x, Q * x) + arma::dot(q, x);
    }

    // This is the gradient of the objective function.
    arma::vec gf(const arma::vec &x) {
        return 2 * (Q * x) + q;
    }

    // This method checks if a point is in the feasible region of the problem.
    bool is_feasible(const arma::vec &x, double eps) {
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
    arma::vec initial_point() {
        arma::vec result = arma::zeros(q.n_elem);
        for (size_t i = 0; i < c.size(); i++) {
            arma::uvec idx = c.at(i);
            result(idx).fill(1.0 / ((double) idx.n_elem));
        }
        return result;
    }

    /*
        This is the implementation of the projected gradient method.
    */
    QResult PGM(const arma::vec &x_0, arma::uword max_iter, double ctol,
    double dtol) {
        auto t1 = std::chrono::high_resolution_clock::now();
        arma::uword k = 0;
        arma::vec x = x_0, g = gf(x);
        arma::uvec act(x.n_elem, arma::fill::zeros);
        std::vector<double> hist;
        while (k < max_iter) {
            hist.push_back(f(x));
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
};

#endif
