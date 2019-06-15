/*
    File:       Utils.cpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#include "Utils.hpp"
#define ARMA_DONT_USE_WRAPPER
#include <armadillo>
#include <iostream>
#include <fstream>

// Loads problem data from a set of CSV files.
QProblem load(std::string path, std::string name) {
    // Build the names.
    std::stringstream name1, name2, name3;
    name1 << path << "/" << name << "_Q.csv";
    name2 << path << "/" << name << "_A.csv";
    name3 << path << "/" << name << "_u.csv";
    // Load Q, A and q from the CSV files.
    arma::mat Q, A;
    arma::vec u;
    Q.load(name1.str(), arma::csv_ascii);
    A.load(name2.str(), arma::csv_ascii);
    u.load(name3.str(), arma::csv_ascii);
    // Build the vector b.
    arma::vec b(A.n_rows, arma::fill::ones);
    QProblem p = {Q, u, A, b};
    return p;
}

// Saves the content of a vector into a file.
void toFile(const std::vector<double> &v, std::string path) {
    std::ofstream output(path);
    for (double x : v) output << x << std::endl;
    output.close();
}
