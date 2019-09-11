#!/bin/bash
#
#   File:     test_n.sh
#   Authors:  Matteo Loporchio, Davide Rucci
#
#   This bash script can be used to test our implementation
#   on problems with dense matrices with variable size n.

TEST_PATH="test_n"
SUFFIX="_c.dat"
SOLVER_PATH="./../qp-solver"
OUTFILE="test_n_alg.csv"
TEST_LIST=( "${TEST_PATH}/*${SUFFIX}" )
REPEAT=5
MAX_ITER="1000"
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
    echo "Processing $J with N=${N}, K=${K}..."
    VAL=0
    COUNT=0
    AVG_IT=0
    AVG_TIME=0
    # Test for REPEAT times.
    while [  $COUNT -lt $REPEAT ]; do
      OUTPUT=$(${SOLVER_PATH} ${TESTCASE} ${MAX_ITER} ${CTOL} ${DTOL} 0)
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
    AVG_TIME=$(echo "scale=6; ${AVG_TIME} / ${REPEAT}" | bc -l)
    # Write the results to the output file.
    echo "${N},${K},${VAL},${AVG_IT},${AVG_TIME},${CTOL},${DTOL}" 1>&3
    J=$((J + 1))
done
