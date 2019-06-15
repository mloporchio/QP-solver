/*
    File:       main.cpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#include "Problem.hpp"
#include "Utils.hpp"
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
    unsigned int maxIter = ((unsigned int) atoi(argv[2]));
    // Line search parameters.
    double alphaInit = atof(argv[3]);
    double tau = atof(argv[4]);
    double beta = atof(argv[5]);
    // Floating-point tolerance.
    double tol = atof(argv[6]);
    // Load the problem from file.
    QProblem p = load(DEFAULT_PATH, name);
    QResult r = PGM(p, maxIter, alphaInit, tau, beta, tol);
    std::cout << "Solution\t: " << r.x.t();
    std::cout << "Value\t\t: " << p.f(r.x) << std::endl;
    std::cout << "Iteration\t: " << r.nIter << std::endl;
    std::cout << "Feasible\t: " << isFeasible(p.A, p.b, r.x, tol) << std::endl;
    //
    toFile(r.history, "data/history.csv");
    return 0;
}
