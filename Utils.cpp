/*
    File:       Utils.cpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#define ARMA_DONT_USE_WRAPPER
#include <armadillo>
#include <iostream>
#include <fstream>
#include "Utils.hpp"

// Loads problem data from a set of CSV files.
QProblem load(std::string path) {
    // Build the names.
    std::stringstream name1, name2, name3;
    name1 << path << "_Q.csv";
    name2 << path << "_A.csv";
    name3 << path << "_u.csv";
    // Load Q, A and q from the CSV files.
    arma::mat Q, A; arma::vec q;
    Q.load(name1.str(), arma::csv_ascii);
    A.load(name2.str(), arma::csv_ascii);
    q.load(name3.str(), arma::csv_ascii);
    // Build the vector b.
    arma::vec b = arma::ones(A.n_rows);
    return {Q, q, A, b};
}

/*
// Saves the content of a vector into a file.
void vectorToFile(const std::vector<double> &v, std::string path) {
    std::ofstream output(path);
    for (double x : v) output << x << std::endl;
    output.close();
}
*/
