#
#   File:		generator.py
#	Authors:	Matteo Loporchio, Davide Rucci
#

import sys
import random
import numpy as np
import pandas as pd

def randomPSMatrix(n):
    Q = np.random.rand(n, n)
    Q = Q * Q.T
    return Q

def randomFRMatrix(k, n):
    A = np.zeros((k, n))
    done = False
    while (not done):
        w = [False for i in range(k)]
        for j in range(0, n):
            i = random.randint(0, k-1)
            w[i] = True
            A[i, j] = 1
        done = np.all(w)
    return A

def main():
    n = int(sys.argv[1])
    k = int(sys.argv[2])
    name = sys.argv[3]
    # Create the matrix Q.
    Q = randomPSMatrix(n)
    # Create the matrix A.
    A = randomFRMatrix(k, n)
    # Create the vector u.
    u = np.random.rand(n, 1)
    #
    pd.DataFrame(Q).to_csv(name + '_Q.csv', header=None, index=False)
    pd.DataFrame(A).to_csv(name + '_A.csv', header=None, index=False)
    pd.DataFrame(u).to_csv(name + '_u.csv', header=None, index=False)

if __name__ == '__main__':
    main()
