/*
    File:       Problem.cpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#include "Problem.hpp"

// Performs a line search to find the step size.
double lineSearch(QProblem &P, double alpha_0, double tau, double beta,
const arma::vec &x, const arma::vec &g, const arma::vec &d) {
    double alpha = alpha_0;
    double z = arma::dot(g, d);
    while (P.f(x + alpha * d) > P.f(x) + alpha * beta * z) alpha *= tau;
    return alpha;
}

// Utility function that performs an implicit binary search for the
// simplex projection.
// Many thanks to: http://www.mcduplessis.com/index.php/2016/08/22/fast-projection-onto-a-simplex-python/
arma::uword binary_search(const arma::vec &a, const arma::vec &x,
const arma::vec &u, const arma::uvec &idx) {
    auto value = [&](arma::uword k) {
        double sum = 0;
        for (arma::uword i = k; i < x.n_elem; i++) {
            sum += (a(idx(i)) * (x(idx(i)) - u(idx(k)) * a(idx(i))));
        }
        return sum - 1;
    };
    arma::uword low = 0, high = u.n_elem - 1;
    double lval = value(low);
    double hval = value(high);
    if (lval < 0) return low;
    while ((high - low) > 1) {
        arma::uword mid = (low + high) / 2;
        double mval = value(mid);
        if (mval > 0) {
            low = mid;
            lval = mval;
        }
        else {
            high = mid;
            hval = mval;
        }
    }
    return high;
}

// Projection of point x onto a generic simplex a^T * x = 1, x >= 0.
// Many thanks to: http://www.mcduplessis.com/index.php/2016/08/22/fast-projection-onto-a-simplex-python/
arma::vec simplex_proj(const arma::vec &a, const arma::vec &x) {
    arma::vec u = arma::zeros(x.n_elem), z = arma::zeros(x.n_elem);
    for (arma::uword i = 0; i < u.n_elem; i++) {
        u(i) = ((a(i) != 0) ? (x(i) / a(i)) : 0);
    }
    arma::uvec idx = arma::sort_index(u);
    arma::uword k = binary_search(a, x, u, idx);
    double s1 = 0, s2 = 0;
    for (arma::uword i = k; i < x.n_elem; i++) {
        double v = a(idx(i));
        s1 += v * x(idx(i));
        s2 += v;
    }
    double l = (s1 - 1) / s2;
    return arma::max(z, x - l * a);
}

// Projects a point x onto the feasible region of the problem P.
arma::vec project(QProblem &P, const arma::vec &x) {
    arma::vec result = arma::zeros(x.n_elem);
    P.A.each_row([&](arma::rowvec &v) {result += simplex_proj(v.t(), x);});
    return result;
}

// Implements the stopping criterion of the PGM.
bool stopping(const arma::vec &x, const arma::vec &d, double atol,
double rtol) {
    return arma::approx_equal(x, d, "both", atol, rtol);
}

// This is the implementation of the projected gradient method.
QResult PGM(QProblem &P, const arma::vec &x_0, double alpha_0, double tau,
double beta, unsigned int max_iter, double atol, double rtol) {
    unsigned int k = 0;
    arma::vec x = x_0;
    while (k < max_iter) {
        double value = P.f(x);
        arma::vec g = P.gf(x);
        arma::vec d = project(P, x - g);
        if (stopping(x, d, atol, rtol)) break;
        else d = d - x;
        double alpha = lineSearch(P, alpha_0, tau, beta, x, g, d);
        x = x + alpha * d;
        k++;
    }
    return {x, P.f(x), k};
}
