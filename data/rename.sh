#!/bin/bash
#
#   File:     rename.sh
#   Author:   Matteo Loporchio
#
#   This script can be used to rename all the .txt files into .dat files.
#

for f in *.txt; do
    mv -- "$f" "${f%.txt}.dat"
done
