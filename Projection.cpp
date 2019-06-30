/*
    File:       Projection.cpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#include <cmath>
#include "Projection.hpp"

// Checks if a vector x is on the unit simplex.
bool on_unit_simplex(const arma::vec &x, double eps) {
    return ((abs(arma::accu(x) - 1) < eps) && arma::all(x >= 0));
}

/*
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
arma::vec generic_simplex_proj(const arma::vec &a, const arma::vec &x) {
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
arma::vec project_full(QProblem &P, const arma::vec &x) {
    arma::vec result = arma::zeros(x.n_elem);
    P.A.each_row([&](arma::rowvec &v) {
        result += generic_simplex_proj(v.t(), x);
    });
    return result;
}
*/

// Projection of point x onto the unit simplex using sorting.
arma::vec sorting_unit_simplex_proj(const arma::vec &x) {
    // Check if x is already on the simplex.
    if (on_unit_simplex(x)) return x;
    arma::uword k = 0;
    arma::vec u = arma::sort(x, "descend");
    arma::vec c = arma::cumsum(u);
    for (arma::uword i = 0; i < u.n_elem; i++) {
        if (u(i) > ((c(i)-1)/(i+1))) k = i+1;
    }
    double t = (c(k-1)-1)/k;
    return arma::max(arma::zeros(x.n_elem), x - t);
}

// Projection of point x onto the unit simplex using Michelot's algorithm.
arma::vec michelot_unit_simplex_proj(const arma::vec &x) {
    // Check if x is already on the simplex.
    if (on_unit_simplex(x)) return x;
    arma::vec v = x;
    arma::uword prev_size = 0;
    double rho = (arma::sum(x) - 1) / x.n_elem;
    while (v.n_elem != prev_size) {
        prev_size = v.n_elem;
        arma::uvec idx = arma::find(v > rho);
        v = v(idx);
        rho = (arma::sum(v) - 1) / v.n_elem;
    }
    double t = rho;
    //arma::uword k = v.n_elem;
    return arma::max(arma::zeros(x.n_elem), x - t);
}

// Projects a point x onto the feasible region of the problem P.
arma::vec project_block(QProblem &P, const arma::vec &x) {
    arma::vec result = arma::zeros(x.n_elem);
    P.A.each_row([&](arma::rowvec &v) {
        arma::uvec idx = arma::find(v > 0);
        result(idx) = sorting_unit_simplex_proj(x(idx));
    });
    return result;
}
