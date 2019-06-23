"""
    File:       solver.py
    Authors:    Matteo Loporchio, Davide Rucci

    This is the main file of the project. It contains the implementation
    of the projected gradient algorithm used to solve our quadratic problem.
"""

import sys
import numpy as np
import scipy as sp
from scipy.optimize import linprog
from numpy.linalg import norm, inv

class QProblem:
    """
    This class represents our quadratic problem, defined as:

        min f(x) = x^T * Q * x + q^T * x
        s.t. Ax = b, x >= 0
    """

    def __init__(self, Q, q, A):
        """Creates a new problem with the given values and constraints."""
        self.Q = Q
        self.q = q
        self.A = A
        self.b = np.ones(np.size(self.A, 0))

    def f(self, x):
        """This is the objective function of the problem."""
        r = np.dot(self.Q, x)
        return np.inner(x, r) + np.inner(self.q, x)

    def gf(self, x):
        """This is the gradient of the objective function."""
        return (2 * np.dot(self.Q, x)) + self.q

    def feasible(self, x, tol):
        """
        Returns true if x belongs to the feasible region of the problem,
        within a given tolerance.
        """
        y = np.dot(self.A, x)
        return (np.allclose(y, self.b, atol=tol) and np.alltrue(x >= 0))


def loadProblem(path):
    """Loads a problem from a set of CSV files."""
    Q = np.loadtxt(path + '_Q.csv', delimiter=',')
    A = np.loadtxt(path + '_A.csv', delimiter=',')
    q = np.loadtxt(path + '_u.csv', delimiter=',')
    return QProblem(Q, q, A)

def initialPoint(P):
    """Returns a starting point for the PGM by solving a linear program."""
    c = np.ones(np.size(P.Q, 0))
    res = linprog(c, A_eq = P.A, b_eq = P.b, bounds=((0, None)))
    return res.x

def lineSearch(P, a_0, t, b, x, g, d):
    """Performs backtracking line search using Armijo."""
    a = a_0
    z = np.inner(g, d)
    while (P.f(x + a * d) > P.f(x) + a * b * z): a *= t
    return a

def simplex_proj(a, y):
    l = np.divide(y, a, out=np.zeros_like(y), where=(a > 0))
    idx = np.argsort(l)
    d = len(l)
    evalpL = lambda k: np.sum(a[idx[k:]]*(y[idx[k:]] - l[idx[k]]*a[idx[k:]]) ) -1
    def bisectsearch():
        idxL, idxH = 0, d-1
        L = evalpL(idxL)
        H = evalpL(idxH)
        if L<0: return idxL
        while (idxH-idxL)>1:
            iMid = int((idxL+idxH)/2)
            M = evalpL(iMid)
            if M>0: idxL, L = iMid, M
            else: idxH, H = iMid, M
        return idxH
    k = bisectsearch()
    lam = (np.sum(a[idx[k:]]*y[idx[k:]])-1)/np.sum(a[idx[k:]])
    x = np.maximum(0, y-lam*a)
    return x

def project(P, x):
    """
    Projects a point x onto the feasible region of the problem P.
    """
    fproj = lambda v: simplex_proj(v, x)
    return np.sum(np.apply_along_axis(fproj, 1, P.A), axis=0)


def stopping(x, d, tol):
    """
    Implements the stopping criterion of the PGM.
    """
    return np.allclose(x, d, atol=tol)


def PGM(P, x0, a0, t, b, tol, max_it):
    """
    This is the implementation of the projected gradient method.
    """
    k = 0
    x = x0
    while k < max_it:
        value = P.f(x)
        grad = P.gf(x)
        d = project(P, x-grad)
        if (stopping(x, d, tol)): break
        else: d = d - x
        alpha = lineSearch(P, a0, t, b, x, grad, d)
        x = x + alpha * d
        k += 1
    return (x, P.f(x), k)

def main():
    path = sys.argv[1]
    max_it = int(sys.argv[2])
    a0 = float(sys.argv[3])
    tau = float(sys.argv[4])
    beta = float(sys.argv[5])
    tol = 1e-12
    P = loadProblem(path)
    x0 = initialPoint(P)
    result = PGM(P, x0, a0, tau, beta, tol, max_it)
    print 'Solution\t=', result[0]
    print 'Feasible\t=', P.feasible(result[0], tol)
    print 'Value\t\t=', result[1]
    print 'Iterations\t=', result[2]

if __name__ == '__main__':
    main()
