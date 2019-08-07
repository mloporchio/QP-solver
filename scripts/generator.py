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

def randomPSMatrix(n, ecc = 0.5):
    """
    Generates a random positive semidefinite n x n matrix.
    """
    Q = np.random.rand(n, n)
    Q = np.matmul(Q.T, Q)
    d, v = np.linalg.eig(Q)
    # Sort the eigenvalues from smallest to largest (and the eigvecs accordingly)
    idx = d.argsort()
    d = d[idx]
    v = v[:, idx]

    # Adjust the eigenvalues to match the eccentricity required
    l = d[0] * np.ones(n) + ( d[0] / (d[n-1] - d[0]) ) * (2 * ecc / (1 - ecc)) * (d - d[0] * np.ones(n))

    # Create the new, adjusted, matrix
    Q = np.matmul(np.matmul(v, np.diag(l)), v.T)

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
    print len(sys.argv)
    if (len(sys.argv) > 4):
        try:
            ecc = float(sys.argv[4])
        except ValueError:
            ecc = None
    else:
        ecc = None
    if (ecc is None):
        print("Using default value for eccentricity: 0.5.")
        ecc = 0.5
    elif (ecc < 0 or ecc >= 1):
        raise InputError('Eccentricity must be between 0 and 1 (excluded).')

    # Create the matrix Q.
    Q = randomPSMatrix(n, ecc)
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
