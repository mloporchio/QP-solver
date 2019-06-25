/*
    File:       Utils.hpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#ifndef UTILS_H
#define UTILS_H

#include "Problem.hpp"

// Loads problem data from a set of CSV files.
QProblem load(std::string path);

// Saves the content of a vector into a file.
// void vectorToFile(const std::vector<double> &v, std::string path);

#endif
