/*
    File:       Utils.hpp
    Authors:    Matteo Loporchio, Davide Rucci

    Computational Mathematics for Learning and Data Analysis 2018/2019
*/

#ifndef UTILS_H
#define UTILS_H

#include <exception>
#include "Problem.hpp"
#include "Config.hpp"

// This struct is used to represent the paths of the files of a problem.
struct pfile_t {
    std::string mat_f; // Path of the matrix.
    std::string vec_f; // Path of the vector.
    std::string cst_f; // Path of the constraint list.
};

// Checks if a given file exists.
bool file_exists(std::string path);

//
pfile_t check_files(std::string path, bool sparse);

// Loads the constraints of the problem the corresponding file.
cstlist_t load_constraints(const std::string &path,
const std::string &delimiter = ",");

// Loads a problem with a dense matrix from disk.
QProblem<arma::mat> load_dense(std::string path);

// Loads a problem with a sparse matrix from disk.
QProblem<arma::sp_mat> load_sparse(std::string path);

// Saves the content of a std::vector into a file.
void vector_to_file(const std::vector<double> &v, std::string path);

#endif
