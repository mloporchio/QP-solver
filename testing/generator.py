"""
    File:       generator.py
    Authors:    Matteo Loporchio, Davide Rucci

    This script can be used to produce random data sets for our
    quadratic problem solver.
    Recall the shape of our optimization problem:

    min f(x) = x^T * Q * x + u^T * x
    s.t. Ax = b, x >= 0

    Simply enter a name and the values of n, k, ecc in order to obtain:

        - Q = n x n matrix with eccentricity = ecc.
        - A = k x n constraint matrix.
        - u = n x 1 vector.

    Usage:

        generator.py <name> <n> <k> [<ecc>]

    NOTE: the vector b is not generated since it is a vector of all ones
    whose size can be deduced by looking at the matrix A.
"""

import sys
import random
import numpy as np

def random_matrix(n, ecc = 0.5):
    """
    Generates a random positive semidefinite n x n matrix with a given
    eccentricity.

    If no eccentricity is provided, a default value of 0.5 will be used.
    """
    Q = np.random.rand(n, n)
    # This should guarantee that Q is also positive semidefinite.
    Q = np.matmul(Q.T, Q)
    # Compute the eigenvalues and eigenvectors.
    d, v = np.linalg.eig(Q)
    # Sort the eigenvalues (and eigenvectors) from smallest to largest.
    idx = d.argsort()
    d = d[idx]
    v = v[:, idx]
    # Adjust the eigenvalues to match the required eccentricity.
    r = 2 * ecc / (1 - ecc)
    u = np.ones(n)
    l = d[0] * u + (d[0] / (d[n-1] - d[0])) * r * (d - d[0] * u)
    # Create the new adjusted matrix.
    Q = np.matmul(np.matmul(v, np.diag(l)), v.T)
    return Q

def random_partition(k, n):
    """
    Fast algorithm to generate a set of k positive integers whose sum is n.
    (Source: https://www.oipapio.com/question-909119)
    """
    indices = [-1] + sorted(random.sample(range(n - 1), k - 1)) + [n - 1]
    return [indices[i + 1] - indices[i] for i in range(k)]

def random_constr(k, n):
    """
    Generates a list of k constraints.
    """
    # Initialize the result.
    c = []
    # Randomly shuffle the range of indices [0, n-1].
    idx = range(n)
    random.shuffle(idx)
    # Generate an array of k positive integers that sum up to n.
    # These integers represent the sizes of the partitions.
    size = random_partition(k, n)
    # For each partition, let s be its size.
    # We have to read s elements from the shuffled indices and then
    # add the corresponding constraints to the result.
    low = 0
    for i in range(k):
        l = []
        for j in range(low, low + size[i]): l.append(idx[j])
        c.append(sorted(l))
        low += size[i]
    return c

def write_constr(filename, list):
    """
    Writes the constraint list to a file.
    """
    with open(filename, 'w') as f:
        for s in list: f.write(','.join(str(x) for x in s) + '\n')

def main():
    # Process the input parameters.
    if (len(sys.argv) < 4):
        print("Usage: " + sys.argv[0] + " <name> <n> <k> [<ecc>]")
        exit(1)
    # If there are at least four parameters, then parse them.
    name = sys.argv[1]
    try:
        n = int(sys.argv[2])
        k = int(sys.argv[3])
    except ValueError:
        print("Error: could not parse one or more numeric arguments.")
        exit(1)
    # Check if an eccentricity has been provided.
    try:
        ecc = float(sys.argv[4])
    except IndexError, ValueError:
        ecc = None
    if (ecc is None):
        print("Using default value for eccentricity = 0.5.")
        ecc = 0.5
    # Check also if the eccentricity is in the range [0, 1).
    elif (ecc < 0 or ecc >= 1):
        raise InputError("Eccentricity must be in the range [0, 1).")
    # Create the matrix Q.
    Q = random_matrix(n, ecc)
    # Create the vector u.
    u = np.random.rand(n, 1)
    # Create the constraint list.
    c = random_constr(k, n)
    # Save the results to separate files.
    np.savetxt(name + '_Q.csv', Q, fmt='%.8f', delimiter=',')
    np.savetxt(name + '_u.csv', u, fmt='%.8f', delimiter=',')
    write_constr(name + '_c.dat', c)

if __name__ == '__main__':
    main()
