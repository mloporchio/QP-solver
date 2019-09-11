===============================================================================

README - Testing

===============================================================================

All the MATLAB functions that are contained in the test subfolders:

  blind_test_dense
  blind_test_sparse
  grid_search_dense
  grid_search_sparse
  sparsity_check
  test_k
  test_n
  variable_density

need to be moved in this folder before being executed, since they depend
on what is defined here. They have been moved into subfolders (together
with the spreadsheets and CSV files) just to keep the code more organized.

Notice that this also holds for the Bash scripts that have been used to
test our implementation, since they rely on the compiled qp-solver
executable which is contained in the main folder of the project.
