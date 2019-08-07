/*
    File:       Utils.cpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include "Utils.hpp"

// Checks if a given file exists.
bool file_exists(std::string path) {
	return ((bool) std::ifstream(path.c_str()));
}

// Loads problem data from a set of CSV files.
// May throw an exception if any of the files does not exist
// or cannot be accessed.
QProblem load_problem(std::string path) {
    // Build the names.
    std::stringstream name1, name2, name3;
    name1 << path << "_Q.csv";
    name2 << path << "_A.csv";
    name3 << path << "_u.csv";
    std::string file1 = name1.str(), file2 = name2.str(), file3 = name3.str();
    // Check if these files exist.
    if (!file_exists(file1) || !file_exists(file2) || !file_exists(file3)) {
		std::stringstream msg;
		msg << "Error while loading data for problem: " << path << std::endl
		<< "Please check if the corresponding files exist.";
		throw std::runtime_error(msg.str());
	}
    // Load Q, A and q from the CSV files.
    arma::mat Q, A; arma::vec q;
    Q.load(file1, arma::csv_ascii);
    A.load(file2, arma::csv_ascii);
    q.load(file3, arma::csv_ascii);
    // Build the vector b.
    arma::vec b = arma::ones(A.n_rows);
    return {Q, q, A, b};
}

// Saves the content of a std::vector into a file.
void vector_to_file(const std::vector<double> &v, std::string path) {
    std::ofstream output(path);
    for (double x : v) {
        output << std::fixed << std::setprecision(FP_PREC) << x << std::endl;
    }
    output.close();
}
