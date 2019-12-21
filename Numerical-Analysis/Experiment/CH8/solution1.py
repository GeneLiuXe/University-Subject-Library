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


def leastSquare(X, Y, n):
    m = len(X)
    R = [[0] * (n + 1) for i in range(m)]
    for i in range(0, m):
        R[i][n] = 1
        for j in range(n - 1, -1, -1):
            R[i][j] = R[i][j + 1] * X[i]
    A = matrixMul(matrixT(R), R)
    b = matrixMul(matrixT(R), Y)
    for i in range(0, n + 1):
        A[i].append(b[i][0])
    return Gaussian(A)


def main():
    n = 2
    X = [-2, -1, 0, 1, 2]
    Y = [[0], [1], [2], [1], [0]]

    n = 1
    ans = leastSquare(X, Y, n)
    for i in range(0, n + 1):
        print(ans[i], end=" ")
    print("\n")

    X2 = np.linspace(-3, 3, 100)
    Y2 = ans[0] * X2 + ans[1]
    plt.plot(X2, Y2, color="rosybrown", linewidth=3.0, linestyle="-", label=r"$P_1(x)$")

    n = 2
    ans = leastSquare(X, Y, n)
    for i in range(0, n + 1):
        print(ans[i], end=" ")
    print("\n")
    X2 = np.linspace(-3, 3, 100)
    Y2 = ans[0] * X2 * X2 + ans[1] * X2 + ans[2]
    plt.plot(X2, Y2, color="darkorange", linewidth=3.0, linestyle="-", label=r"$P_2(x)$")

    n = 3
    ans = leastSquare(X, Y, n)
    for i in range(0, n + 1):
        print(ans[i], end=" ")
    print("\n")
    X2 = np.linspace(-3, 3, 100)
    Y2 = ans[0] * X2 * X2 * X2 + ans[1] * X2 * X2 + ans[2] * X2 + ans[3]
    plt.plot(X2, Y2, color="wheat", linewidth=3.0, linestyle="-.", label=r"$P_3(x)$")

    n = 4
    ans = leastSquare(X, Y, n)
    for i in range(0, n + 1):
        print(ans[i], end=" ")
    print("\n")
    X2 = np.linspace(-3, 3, 100)
    Y2 = ans[0] * X2 * X2 * X2 * X2 + ans[1] * X2 * X2 * X2 + ans[2] * X2 * X2 + ans[3] * X2 + ans[4]
    plt.plot(X2, Y2, color="cornflowerblue", linewidth=3.0, linestyle="-", label=r"$P_4(x)$")

    Y1 = [0, 1, 2, 1, 0]
    plt.scatter(X, Y1, s=100, label="Observed Data")
    plt.legend(loc="upper center")
    plt.tight_layout()
    plt.savefig('plot8-2.pdf', dpi=700)
    plt.show()


if __name__ == "__main__":
    main()
