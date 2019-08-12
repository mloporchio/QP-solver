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

// Loads the constraints of the problem the corresponding file.
cstlist_t load_constraints(const std::string &path,
const std::string &delimiter) {
	std::ifstream input(path);
	std::string line;
	cstlist_t v;
	while (getline(input, line)) {
		// Initialize the container.
		std::vector<arma::uword> r;
		// Parse the line and collect the tokens.
		auto start = 0;
        auto end = line.find(delimiter);
        while (end != std::string::npos) {
			r.push_back((arma::uword) stoi(line.substr(start, end - start)));
            start = end + delimiter.length();
            end = line.find(delimiter, start);
        }
		r.push_back((arma::uword) stoi(line.substr(start, end)));
		// Build the vector.
		v.push_back(arma::uvec(r));
	}
	return v;
}

// Loads problem data from a set of CSV files.
// May throw an exception if any of the files does not exist
// or cannot be accessed.
QProblem load_problem(std::string path) {
    // Build the names.
    std::stringstream name1, name2, name3;
    name1 << path << "_Q.csv";
    name2 << path << "_u.csv";
    name3 << path << "_c.txt";
    std::string file1 = name1.str(), file2 = name2.str(), file3 = name3.str();
    // Check if these files exist.
    if (!file_exists(file1) || !file_exists(file2) || !file_exists(file3)) {
		std::stringstream msg;
		msg << "Error while loading data for problem: " << path << std::endl
		<< "Please check if the corresponding files exist.";
		throw std::runtime_error(msg.str());
	}
    // Load Q and q from the CSV files.
    arma::mat Q;
	arma::vec q;
    Q.load(file1, arma::csv_ascii);
    q.load(file2, arma::csv_ascii);
	// Load the constraint list.
	cstlist_t c = load_constraints(file3);
	// Build and return the struct.
    return {Q, q, c};
}

// Saves the content of a std::vector into a file.
void vector_to_file(const std::vector<double> &v, std::string path) {
    std::ofstream output(path);
    for (double x : v) {
        output << std::fixed << std::setprecision(FP_PREC) << x << std::endl;
    }
    output.close();
}
