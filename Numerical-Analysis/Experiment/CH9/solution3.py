import math
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import style

style.use('ggplot')
plt.figure(figsize=(10, 10), dpi=80)
style_list = ["g+-", "r--", "b.-", "yo-"]


def matrixT(A):
    n = len(A)
    m = len(A[0])
    B = [[0] * n for i in range(m)]
    for i in range(0, m):
        for j in range(0, n):
            B[i][j] = A[j][i]
    return B


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


def norm2(A):
    tmp = 0
    for i in range(0, len(A)):
        tmp += A[i][0] * A[i][0]
    return np.sqrt(tmp)


def Gaussian(A):
    n = len(A)

    for i in range(0, n):
        # Search for maximum in this column
        maxEl = abs(A[i][i])
        maxRow = i
        for k in range(i + 1, n):
            if abs(A[k][i]) > maxEl:
                maxEl = abs(A[k][i])
                maxRow = k

        # Swap maximum row with current row
        for k in range(i, n + 1):
            A[maxRow][k], A[i][k] = A[i][k], A[maxRow][k]

        # Make all rows below this one 0 in current column
        for k in range(i + 1, n):
            c = -A[k][i] / A[i][i]
            for j in range(i, n + 1):
                if i == j:
                    A[k][j] = 0
                else:
                    A[k][j] += c * A[i][j]

    # Solve equation Ax=b for an upper triangular matrix A
    x = [0 for i in range(n)]
    for i in range(n - 1, -1, -1):
        x[i] = A[i][n] / A[i][i]
        for k in range(i - 1, -1, -1):
            A[k][n] -= A[k][i] * x[i]
    return x


def sympower(A, X, n, TOL):
    base, value = [], []
    for k in range(0, n):
        y = matrixMul(A, X)
        T1 = matrixMul(matrixT(X), y)
        u = T1[0][0]
        yp = norm2(y)
        if yp == 0:
            print("Eigenvector", end=" ")
            for i in range(0, len(X)):
                print(X[i][0], end=" \n"[0 == len(X) - 1])
            print("A has the eigenvalue 0, select a new vector x and restart")
        err = 0
        for i in range(0, len(A)):
            err += (X[i][0] - y[i][0] / yp) * (X[i][0] - y[i][0] / yp)
        err = np.sqrt(err)
        base.append(k+1)
        value.append(u)
        for i in range(0, len(A)):
            X[i][0] = y[i][0] / yp
        if err < TOL:
            print("Eigenvalue", u, "times", k)
            print("Eigenvector", end=" ")
            for i in range(0, len(X)):
                print(X[i][0], end=" \n"[0 == len(X) - 1])
            break
    plt.plot(base, value, '-p', color='grey',
             marker='o',
             markersize=8, linewidth=2,
             markerfacecolor='red',
             markeredgecolor='grey',
             markeredgewidth=2)

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
        base.append(k)
        value.append(u)
        if err < TOL:
            print("Eigenvalue", u)
            print("Eigenvector", end=" ")
            for i in range(0, len(X)):
                print(X[i][0], end=" \n"[0 == len(X) - 1])
            break
        for i in range(0, len(X)):
            X[i][0] = y[i][0] / u
    plt.plot(base, value, '-p', color='grey',
             marker='o',
             markersize=8, linewidth=2,
             markerfacecolor='red',
             markeredgecolor='grey',
             markeredgewidth=2)


def main():
    A = [[4, -1, 1], [-1, 3, -2], [1, -2, 3]]
    X = [[1], [1], [1]]
    sympower(A, X, 50, 1e-8)
    # power(A, X, 50, 1e-8)
    # plt.legend(loc="upper center")
    plt.tight_layout()
    plt.savefig('plot9-2.pdf', dpi=700)
    plt.show()


if __name__ == "__main__":
    main()
