#!/bin/bash
#
#     File:     grid_search_sparse.sh
#     Authors:  Matteo Loporchio, Davide Rucci
#
#     This script can be used to run the grid search on problems
#     with a sparse Hessian. The test cases are solved using our PGM
#     and the corresponding results are written to a file.

SUFFIX="_Q.csv"
OUTFILE="grid_search_sparse_alg.csv"
TESTLIST=$(ls grid_search_sparse/*"$SUFFIX")
TOLS="1E-6 1E-8 1E-12 1E-16"
MAX_ITER="10000"

# Open the output file.
exec 3>> $OUTFILE
# For each test case...
for TESTCASE_P in $TESTLIST; do
    # Obtain the name of the data set.
    TESTCASE=${TESTCASE_P%"$SUFFIX"}
    # Obtain the values of the parameters.
    N=$(echo $TESTCASE | cut -d"_" -f 4)
    K=$(echo $TESTCASE | cut -d"_" -f 5)
    DEN=$(echo $TESTCASE | cut -d"_" -f 6)
    DEN=${DEN%"d"}
    echo "Processing $TESTCASE with N=${N}, K=${K}, DEN=${DEN}..."
    # Try all possible combinations of tolerances...
    for CTOL in $TOLS; do
      for DTOL in $TOLS; do
        # If density is less than 0.5, load as a sparse matrix.
        if (( $(echo "${DEN} <= 0.5" | bc -l) )); then
          OUTPUT=$(./../qp-solver ${TESTCASE} ${MAX_ITER} ${CTOL} ${DTOL} 1)
        else
          OUTPUT=$(./../qp-solver ${TESTCASE} ${MAX_ITER} ${CTOL} ${DTOL} 0)
        fi
        # Process the output.
        VAL=$(echo $OUTPUT | cut -d " " -f 3)
        IT=$(echo $OUTPUT | cut -d " " -f 6)
        TIME=$(echo $OUTPUT | cut -d " " -f 12)
        # Transform the duration into milliseconds.
        TIME=$(echo "scale=6; $TIME / 1000" | bc -l)
        echo "${N},${K},${DEN},${VAL},${IT},${TIME},${CTOL},${DTOL}"
        # Write the results to a file.
        echo "${N},${K},${DEN},${VAL},${IT},${TIME},${CTOL},${DTOL}" 1>&3
      done
    done
done
