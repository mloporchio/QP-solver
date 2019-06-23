"""
    File:       generator.py
    Authors:    Matteo Loporchio, Davide Rucci

    This script can be used to produce random data sets
    for our quadratic problem solver.
    Recall the shape of our optimization problem:

    min f(x) = x^T * Q * x + u^T * x
    s.t. Ax = b, x >= 0

    Simply enter the values of n, k and a name in order to obtain:

        - Q = n x n matrix.
        - A = k x n constraint matrix.
        - u = n x 1 vector.

    NOTE: the vector b is not generated since it is a vector of all ones
    whose size can be deduced by looking at the matrix A.
"""

import sys
import random
import numpy as np

def randomPSMatrix(n):
    """
    Generates a random positive semidefinite n x n matrix.
    """
    Q = np.random.rand(n, n)
    Q = np.matmul(Q, Q.T)
    return Q

def randomFRMatrix(k, n):
    """
    Generates the k x n constraint matrix with full row rank.
    """
    A = None
    done = False
    while (not done):
        A = np.zeros((k, n))
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
    # Save the results to files.
    np.savetxt(name + '_Q.csv', Q, fmt='%.6f', delimiter=',')
    np.savetxt(name + '_A.csv', A, fmt='%.6f', delimiter=',')
    np.savetxt(name + '_u.csv', u, fmt='%.6f', delimiter=',')

if __name__ == '__main__':
    main()
