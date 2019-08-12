/*
    File:       Utils.hpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#ifndef UTILS_H
#define UTILS_H

#include <exception>
#include "Problem.hpp"

// Checks if a given file exists.
bool file_exists(std::string path);

// Loads the constraints of the problem the corresponding file.
cstlist_t load_constraints(const std::string &path,
const std::string &delimiter = ",");

// Loads problem data from a set of CSV files.
// May throw an exception if any of the files does not exist
// or if it can't be accessed.
QProblem load_problem(std::string path);

// Saves the content of a std::vector into a file.
void vector_to_file(const std::vector<double> &v, std::string path);

#endif
