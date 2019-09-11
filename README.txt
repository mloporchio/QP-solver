===============================================================================

README - QP-solver

===============================================================================

This is a convex quadratic program solver written in C++ and
based on Rosen's projected gradient method.

To compile the executable, just type:

  make all

To run the algorithm, call the executable from a command line with
the following syntax

  ./qp-solver <name> <max_iter> <ctol> <dtol> <sparse>

where:

  - <name> is the path of the data set to be loaded.

  - <max_iter> is the maximum number of iterations the algorithm should do.

  - <ctol> is the constraint tolerance, i.e. the threshold under which
  a constraint is considered active.

  - <dtol> is the norm tolerance, i.e. the threshold on the norm of
  the projected gradient. The algorithm will stop when the norm is below
  this threshold.

  - <sparse> is a boolean parameter (with 0/1 values) which defines
  if the current data set should be loaded as a sparse problem.
  To achieve the best performance, this should be set to 1 whenever
  the Hessian of the quadratic problem has a density <= 50 %.

NOTICE: The compilation process has been tested successfully on the latest
version of macOS.
