#!/bin/bash
#
#   File:     sparsity_check.sh
#   Authors:  Matteo Loporchio, Davide Rucci
#
#   This script can be used to test how our implementation behaves
#   once n and k are fixed and the density of the Hessians is changing.


TEST_PATH="sparsity_check"
SUFFIX="_c.dat"
SOLVER_PATH="./../qp-solver"
OUTFILE="sparsity_check_output.csv"
TEST_LIST=( "${TEST_PATH}/*${SUFFIX}" )
REPEAT=5
MAX_ITER="10000"
CTOL="1E-8"
DTOL="1E-6"
J=1

# Open the output file.
exec 3>$OUTFILE
# Process each test case.
for TESTCASE_P in $TEST_LIST; do
    # Obtain the name of the data set.
    TESTCASE=${TESTCASE_P%"$SUFFIX"}
    # Obtain the values of the parameters.
    N=$(echo $TESTCASE | cut -d "_" -f 3)
    K=$(echo $TESTCASE | cut -d "_" -f 4)
    DEN=$(echo $TESTCASE | cut -d "_" -f 5)
    echo "Processing $J with N=${N}, K=${K}, DEN=${DEN}..."
    VAL=0
    COUNT=0
    AVG_IT=0
    AVG_TIME=0
    # Test for REPEAT times.
    while [  $COUNT -lt $REPEAT ]; do
      OUTPUT=$(${SOLVER_PATH} ${TESTCASE} ${MAX_ITER} ${CTOL} ${DTOL} 1)
      # Process the output.
      VAL=$(echo $OUTPUT | cut -d " " -f 3)
      IT=$(echo $OUTPUT | cut -d " " -f 6)
      TIME=$(echo $OUTPUT | cut -d " " -f 12)
      # Scale to milliseconds.
      TIME=$(echo "scale=6; $TIME / 1000" | bc -l)
      # Update the averages.
      AVG_IT=$(echo "${AVG_IT} + ${IT}" | bc -l)
      AVG_TIME=$(echo "${AVG_TIME} + ${TIME}" | bc -l)
      # Increment counter.
      COUNT=$((COUNT + 1))
    done
    AVG_IT=$(echo "scale=3; ${AVG_IT} / ${REPEAT}" | bc -l)
    AVG_TIME=$(echo "scale=3; ${AVG_TIME} / ${REPEAT}" | bc -l)
    # Write the results to the output file.
    echo "${N},${K},${DEN},${VAL},${AVG_IT},${AVG_TIME},${CTOL},${DTOL}" 1>&3
    J=$((J + 1))
done
