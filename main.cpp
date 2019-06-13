/*
    File:       main.cpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#include "Problem.hpp"
#include <iostream>

#define DEFAULT_PATH "./data"

int main(int argc, char **argv) {
    // Read the parameters.
    if (argc < 7) {
        std::cerr << "Usage:" << std::endl
        << argv[0] << " <name> <maxIter> <alphaInit> <tau> <beta> <tol>"
        << std::endl;
        return 1;
    }
    // Name of the data set.
    std::string name(argv[1]);
    // Maximum number of iterations.
    int maxIter = atoi(argv[2]);
    // Line search parameters.
    double alphaInit = atof(argv[3]);
    double tau = atof(argv[4]);
    double beta = atof(argv[5]);
    // Floating-point tolerance.
    double tol = atof(argv[6]);
    // Load the problem from file.
    QProblem r = load(DEFAULT_PATH, name);
    arma::vec z = PGM(r, maxIter, alphaInit, tau, beta, tol);
    std::cout << "Solution = " << z.t();
    std::cout << "Value = " << r.f(z) << std::endl;
    std::cout << "Feasible = " << isFeasible(r.A, r.b, z, tol) << std::endl;
    return 0;
}
