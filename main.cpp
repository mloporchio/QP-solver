/*
    File:       main.cpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#include "Problem.hpp"
#include "Initial.hpp"
#include "Utils.hpp"
#include <iostream>

int main(int argc, char **argv) {
    // Read the parameters.
    if (argc < 3) {
        std::cerr << "Usage:" << std::endl
        << argv[0] << " <name> <max_iter>"
        << std::endl;
        return 1;
    }
    // Name of the data set.
    std::string path(argv[1]);
    // Maximum number of iterations.
    unsigned int max_iter = ((unsigned int) atoi(argv[2]));
    // Load the problem from file.
    QProblem P = load(path);
    // Set the initial point for the PGM.
    arma::vec x_0 = initial_point_first(P);
    // Solve the problem.
    QResult R = PGM(P, x_0, max_iter);
    // Print the results.
    std::cout << "Solution\t: " << R.x.t();
    std::cout << "Value\t\t: " << P.f(R.x) << std::endl;
    std::cout << "Iteration\t: " << R.n_iter << std::endl;
    std::cout << "Feasible\t: " << P.isFeasible(R.x) << std::endl;
    return 0;
}
