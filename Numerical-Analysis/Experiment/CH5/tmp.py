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


def inversePower(A, X, n, TOL):
    T1 = matrixMul(matrixMul(matrixT(X), A), X)
    T2 = matrixMul(matrixT(X), X)
    q = T1[0][0] / T2[0][0]
    for k in range(0, n):
        B = [[0] * (len(A[0]) + 1) for i in range(len(A))]
        for i in range(0, len(A)):
            for j in range(0, len(A[0])):
                B[i][j] = A[i][j]
            B[i][i] -= q
            B[i][len(A[0])] = X[i][0]
        z = Gaussian(B)
        y = [[0] * 1 for i in range(len(z))]
        for i in range(0, len(z)):
            y[i][0] = z[i]
        u = norm(y)
        err = 0
        for i in range(0, len(X)):
            err = max(err, X[i][0] - (y[i][0] / u))
        for i in range(0, len(X)):
            X[i][0] = y[i][0] / u
        if err < TOL:
            u = 1 / u + q
            print("Eigenvalue", u, "times", k + 1)
            print("Eigenvector", end=" ")
            for i in range(0, len(X)):
                print(X[i][0], end=" \n"[0 == len(X) - 1])
            break



def main():
    # A = [[4, -1, 1], [-1, 3, -2], [1, -2, 3]]
    # X = [[1], [1], [1]]
    # inversePower(A, X, 10000, 1e-5)
    X = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23]
    Y = [4.511323,
        5.1133,
        5.4232,
        5.6889,
        5.83444,
        5.9122,
        5.95444,
        5.979090909090909,
        5.983846153846154,
        5.986744186046512,
        5.994326848249027,
        5.998052046783626,
        5.998173170731708,
        5.998535871156661,
        5.99926775689529,
        5.9996338337605275,
        5.999816905706439,
        5.9999084500595075,
        5.999954224331294,
        5.999977111991028,
        5.999988555951859,
        5.9999942779650155,
        5.999997138979779,
        ]
    plt.plot(X, Y, '-p', color='grey',
             marker='o',
             markersize=8, linewidth=2,
             markerfacecolor='red',
             markeredgecolor='grey',
             markeredgewidth=2)
    # power(A, X, 50, 1e-8)
    # plt.legend(loc="upper center")
    plt.tight_layout()
    plt.savefig('plot9-3.pdf', dpi=700)
    plt.show()


if __name__ == "__main__":
    main()
