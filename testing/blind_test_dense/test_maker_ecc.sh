#!/bin/bash
#
#   File:     test_maker_ecc.sh
#   Authors:  Matteo Loporchio, Davide Rucci
#
#   This script can be used to run the blind tests in the eccentricity case.
#   We generate random values for n and k while keeping the eccentricity
#   equal to 0.999. Matlab's quadprog is also called in order to compare
#   its results to our implementation.
#

MAX_N=2000
MIN_N=500
MAX_ITER=10000

CTOL="1E-12" # Ctol desiderata
DTOL="1E-8" # Dtol desiderata

MATLAB_CMD="/Applications/MATLAB_R2019a.app/bin/matlab -nodesktop -nosplash -nojvm -nodisplay" # Percorso dell'eseguibile di matlab

# Crea il csv dei risultati da 0 ogni volta
exec 3>"output_ecc_final.csv"

echo "n, k, den, matlab, iterazioni matlab, tempo matlab, algoritmo, ottimo?, iterazioni, tempo algoritmo, ctol, dtol, note" 1>&3

for ((i=0; i<$1; i+=1)); do
    # Intero casuale tra min e max
    N=$(( MIN_N + ( RANDOM % (MAX_N - MIN_N) ) ))
    K=$(( ( RANDOM % N )  + 1 ))
    ECC="0.999"

    # Genera un nuovo testcase casuale
    TESTCASE="final_test_ecc/test_${N}_${K}_${ECC}" # Nome del testcase
    command python generator.py ${TESTCASE} $N $K 0.999 # Genero il testcase

    SUM="0"
    for j in {0..5}; do # Fa la media su 5 esecuzioni
        # Chiama l'algoritmo sul test appena creato
        OUTPUT=$(./../qp-solver ${TESTCASE} ${MAX_ITER} ${CTOL} ${DTOL} 0)
        OTTIMO=$(echo $OUTPUT | cut -d' ' -f 3)
        ITERAZIONI=$(echo $OUTPUT | cut -d' ' -f 6)
        MICROSEC=$(echo $OUTPUT | cut -d' ' -f 12)
        SEC=$(echo "scale=7;$MICROSEC / 1000000" | bc -l)
        SUM=$(echo "scale=7;$SUM+$SEC" | bc -l)
    done
    SEC=$(echo "scale=7;$SUM / 5" | bc -l)

    MATLAB_OUTPUT=$(command ${MATLAB_CMD} -r "solve_problem(load_problem('${TESTCASE}', 0)), exit;")

    MATLAB_ITER=$(echo $MATLAB_OUTPUT | cut -d'"' -f 4) # Ok, iterazioni matlab
    MATLAB_OPT=$(echo $MATLAB_OUTPUT | cut -d'"' -f 6) # Ok, valore ottimo
    MATLAB_TIME=$(echo $MATLAB_OUTPUT | cut -d'"' -f 2) # Ok, tempo (in secondi)

    # n, k, ecc, matlab, iterazioni matlab, tempo matlab, algoritmo, ottimo?, iterazioni, tempo algoritmo ctol, dtol, note
    echo "${N},${K},${ECC},${MATLAB_OPT},${MATLAB_ITER},${MATLAB_TIME},${OTTIMO},,${ITERAZIONI},${SEC},${CTOL},${DTOL}," 1>&3
    echo "${N},${K},${ECC},${MATLAB_OPT},${MATLAB_ITER},${MATLAB_TIME},${OTTIMO},,${ITERAZIONI},${SEC},${CTOL},${DTOL},"
done
