/*
    File:       main.cpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#include "Problem.hpp"
#include "Utils.hpp"
#include <exception>
#include <iomanip>
#include <iostream>

int main(int argc, char **argv) {
    // Read the parameters.
    if (argc < 6) {
        std::cerr << "Usage:" << std::endl
        << argv[0] << " <name> <max_iter> <ctol> <dtol> <sparse>"
        << std::endl;
        return 1;
    }
    // Name of the data set.
    std::string path(argv[1]);
    // Read the parameters from the command line.
    unsigned int max_iter = ((unsigned int) atoi(argv[2]));
    double ctol = atof(argv[3]), dtol = atof(argv[4]);
    int sparse = atoi(argv[5]);
    // Load and solve the problem.
    QResult R;
    bool feasible = false;
    try {
        if (sparse) {
            QProblem<arma::sp_mat> P = load_sparse(path);
            arma::vec x_0 = P.initial_point();
            R = P.PGM(x_0, max_iter, ctol, dtol);
            feasible = P.is_feasible(R.x, ctol);
        }
        else {
            QProblem<arma::mat> P = load_dense(path);
            arma::vec x_0 = P.initial_point();
            R = P.PGM(x_0, max_iter, ctol, dtol);
            feasible = P.is_feasible(R.x, ctol);
        }
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    // Print the results.
    std::cout << std::fixed << std::setprecision(FP_STDOUT)
    << "Value\t\t= " << R.v << std::endl
    << "Iterations\t= " << R.n_iter << std::endl
    << "Feasible\t= " << feasible << std::endl
    << "Time\t\t= " << R.time << " us" << std::endl;
    // Write the value history to a file.
    #if LOG_VAL
    vector_to_file(R.hist, path + "_val.csv");
    #endif
    return 0;
}
