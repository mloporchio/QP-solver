/*
    File:       main.cpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#include "Problem.hpp"
#include "Utils.hpp"
#include <exception>
#include <iostream>

int main(int argc, char **argv) {
    // Read the parameters.
    if (argc < 5) {
        std::cerr << "Usage:" << std::endl
        << argv[0] << " <name> <max_iter> <cst_tol> <opt_tol>"
        << std::endl;
        return 1;
    }
    // Name of the data set.
    std::string path(argv[1]);
    // Read the parameters from the command line.
    unsigned int max_iter = ((unsigned int) atoi(argv[2]));
    double ctol = atof(argv[3]), dtol = atof(argv[4]);
    // Load the problem from file.
    QProblem P;
    try {
        P = load_problem(path);
    }
    // Check if something went wrong during the process.
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    // Set the initial point for the PGM.
    arma::vec x_0 = P.initial_point();
    // Solve the problem.
    QResult R = PGM(P, x_0, max_iter, ctol, dtol);
    // Print the results.
    std::cout
    /*<< "Solution\t= " << R.x.t()*/
    << "Value\t\t= " << R.v << std::endl
    << "Iteration\t= " << R.n_iter << std::endl
    << "Feasible\t= " << P.isFeasible(R.x, ctol)
    << std::endl;
    return 0;
}
