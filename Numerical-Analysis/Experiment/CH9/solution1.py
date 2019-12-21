import math
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import style

style.use('ggplot')
plt.figure(figsize=(10, 10), dpi=80)
style_list = ["g+-", "r--", "b.-", "yo-"]


def matrixMul(A, B):
    n = len(A)
    m = len(B[0])
    L = len(B)
    C = [[0] * m for i in range(n)]
    for i in range(0, n):
        for j in range(0, m):
            for k in range(0, L):
                C[i][j] += A[i][k] * B[k][j]
    return C


def norm(A):
    tmp = 0
    for i in range(0, len(A)):
        tmp = max(tmp, A[i][0])
    return tmp


def power(A, X, n, TOL):
    base, value = [], []
    for k in range(0, n):
        y = matrixMul(A, X)
        u = norm(y)
        if u == 0:
            print("Eigenvector", end=" ")
            for i in range(0, len(X)):
                print(X[i][0], end=" \n"[0 == len(X) - 1])
            print("A has the eigenvalue 0, select a new vector x and restart")
        err = 0
        for i in range(0, len(X)):
            err = max(err, X[i][0] - (y[i][0] / u))
        base.append(k+1)
        value.append(u)
        if err < TOL:
            print("Eigenvalue", u, "times", k+1)
            print("Eigenvector", end=" ")
            for i in range(0, len(X)):
                print(X[i][0], end=" \n"[0 == len(X) - 1])
            break
        for i in range(0, len(X)):
            X[i][0] = y[i][0] / u
    for i in range(0,len(value)):
        print(value[i])
    plt.plot(base, value, '-p', color='grey',
             marker='o',
             markersize=8, linewidth=2,
             markerfacecolor='red',
             markeredgecolor='grey',
             markeredgewidth=2)


def main():
    A = [[4, -1, 1], [-1, 3, -2], [1, -2, 3]]
    X = [[1], [1], [1]]
    power(A, X, 50, 1e-8)
    # plt.legend(loc="upper center")
    plt.tight_layout()
    plt.savefig('plot9-1.pdf', dpi=700)
    plt.show()


if __name__ == "__main__":
    main()
