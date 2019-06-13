/*
    File:       generator.cpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#include "Utils.hpp"
#include <sstream>
#include <string>

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cerr << "Usage:" << std::endl
        << argv[0] << " <n> <k> <name>" << std::endl;
        return 1;
    }
    // Read the parameter values.
    int n = atoi(argv[1]);
    int k = atoi(argv[2]);
    std::string base(argv[3]);
    //
    std::stringstream name1, name2, name3;
    name1 << base << "_Q.csv";
    name2 << base << "_A.csv";
    name3 << base << "_u.csv";
    // Generate a random positive semidefinite matrix.
    arma::mat Q = randomPSMatrix(n);
    Q.save(name1.str(), arma::csv_ascii);
    // Generate the full rank constraint matrix.
    arma::mat A = randomFRMatrix(k, n);
    A.save(name2.str(), arma::csv_ascii);
    //
    arma::vec q(n, arma::fill::randn);
    q.save(name3.str(), arma::csv_ascii);
    //
    return 0;
}
