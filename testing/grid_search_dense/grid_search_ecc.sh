#!/bin/bash
#
#   File:     grid_search_ecc.sh
#   Authors:  Matteo Loporchio, Davide Rucci
#
#   This script can be used to run a grid search in order to determine
#   the best algorithmic parameters for our implementation.
#   Here we test our PGM on matrices having different eccentricities.

TESTLIST=$(ls grid_search_ecc/*_Q.csv)
MATLAB_CMD="/Applications/MATLAB_R2019a.app/bin/matlab -nodisplay -nojvm -nodesktop -nosplash"

TOLS="1E-7 1E-8 1E-12 1E-15"
MAX_ITER="10000"


for TESTCASE_P in $TESTLIST; do
    TESTCASE=${TESTCASE_P//_Q.csv}
    N=$(echo $TESTCASE | cut -d"_" -f 4)
    K=$(echo $TESTCASE | cut -d"_" -f 5)
    ECC=$(echo $TESTCASE | cut -d"_" -f 6)

    OUTPUTFILE="output_ecc_${N}.csv"
    exec 3>>$OUTPUTFILE

    MATLAB_OUTPUT=$(command ${MATLAB_CMD} -r "sprintf('%.6f', timeit(@() solve_problem(load_problem('${TESTCASE}', 0)))), exit;")

    MATLAB_OPT=$(echo $MATLAB_OUTPUT | cut -d'"' -f 2) # Ok, valore ottimo
    MATLAB_TIME=$(echo $MATLAB_OUTPUT | cut -d"'" -f 2) # Ok, tempo (in secondi)

    for CTOL in $TOLS; do
        for DTOL in $TOLS; do
            OUTPUT=$(./../qp-solver ${TESTCASE} ${MAX_ITER} ${CTOL} ${DTOL} 0)
            OTTIMO=$(echo $OUTPUT | cut -d' ' -f 3)
            ITERAZIONI=$(echo $OUTPUT | cut -d' ' -f 6)
            MICROSEC=$(echo $OUTPUT | cut -d' ' -f 12)
            SEC=$(echo "scale=7;$MICROSEC / 1000000" | bc -l)
            # n, k, ecc, matlab, tempo matlab, algoritmo, ottimo?, iterazioni, tempo algoritmo ctol, dtol, note
            echo "${N},${K},${ECC},${MATLAB_OPT},${MATLAB_TIME},${OTTIMO},,${ITERAZIONI},${SEC},${CTOL},${DTOL}," 1>&3
            echo "${N},${K},${ECC},${MATLAB_OPT},${MATLAB_TIME},${OTTIMO},,${ITERAZIONI},${SEC},${CTOL},${DTOL},"
            done
    done
done
