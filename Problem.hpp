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
template <typename T>
class QProblem {
private:
    T Q;            // This is the Hessian matrix of the quadratic function.
    arma::vec q;    // This is the linear part.
    cstlist_t c;    // This is the list of constraints.

    // Computes the currently active constraints.
    void set_active(const arma::vec &x, arma::uvec &act, double eps);

    // Computes the maximum feasible stepsize.
    double max_step(const arma::vec &x, const arma::vec &d,
    const arma::uvec &act, double eps);

    // Performs a line search in the interval [0, alpha_bar].
    double line_search(const arma::vec &g, const arma::vec &d,
    const arma::vec &l, double alpha_bar, double eps = 1E-16);

    // Computes the projection of the gradient onto the subspace A_bar * d = 0.
    arma::vec project(const arma::uvec &act, const arma::vec &g);

    // Projects the gradient onto the single block of coordinates.
    arma::vec sub_proj(const arma::vec &g, const arma::uvec &act);

public:
    // Default constructor.
    QProblem<T>() {};

    // Main constructor.
    QProblem<T>(T &Q, arma::vec &q, cstlist_t &c);

    // This is the objective function.
    double f(const arma::vec &x);

    // This is the gradient of the objective function.
    arma::vec gf(const arma::vec &x);

    // This method checks if a point is in the feasible region of the problem.
    bool is_feasible(const arma::vec &x, double eps);

    // Returns a feasible starting point where each component
    // of a partition S is equal to 1 / # of elements in S.
    arma::vec initial_point();

    /*
        This is the implementation of the projected gradient method.
        The following input parameters are required:

            - x_0, the initial feasible point.
            - max_iter, a limit on the number of iterations.
            - ctol, constraint tolerance.
            - dtol, norm tolerance.
    */
    QResult PGM(const arma::vec &x_0, arma::uword max_iter,
    double ctol, double dtol);
};

// The actual implementation is defined in the file Problem.cpp.
#include "Problem.cpp"

#endif
