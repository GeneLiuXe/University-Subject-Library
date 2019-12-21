import numpy as np
import random
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import style
from copy import deepcopy as dco

style.use('ggplot')

import matplotlib

matplotlib.rcParams['text.usetex'] = True

plt.figure(figsize=(10, 10), dpi=70)
style_list = ["g+-", "r--", "b.-", "yo-"]

l, r, X, Y = 0, np.pi, [], []


def drawFunction(L, R, A, B, C, D, lineName, col, lwidth, style):
    x = np.linspace(L, R, 10)
    y = A + B * (x - L) + C * (x - L) * (x - L) + D * (x - L) * (x - L) * (x - L)
    if L == l:
        plt.plot(x, y, color=col, linewidth=lwidth, linestyle=style, label=lineName)
    else:
        plt.plot(x, y, color=col, linewidth=lwidth, linestyle=style)

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


def func(x):
    return np.sin(x)


def func2(x):
    return np.cos(x)


def spline(A, B, C, D, H, G, n, lineName, col, lwidth, style):
    C = Gaussian(G)
    for i in range(0, n):
        D.append((C[i + 1] - C[i]) / (3 * H[i]))
        B.append((A[i + 1] - A[i]) / H[i] - H[i] * (2 * C[i] + C[i + 1]) / 3)
    for i in range(0, n):
        drawFunction(X[i], X[i + 1], A[i], B[i], C[i], D[i], lineName, col, lwidth, style)


def natural(G, A, H, n):
    G[n - 1][0] = 1
    G[n][n] = 1
    return G


def clamped(G, A, H, n):
    G[n - 1][0] = 2 * H[0]
    G[n - 1][1] = H[0]
    G[n - 1][n + 1] = 3 * (A[1] - A[0]) / H[0] - 3 * func2(X[0])

    G[n][n - 1] = H[n - 1]
    G[n][n] = 2 * H[n - 1]
    G[n][n + 1] = 3 * func2(X[n]) - 3 * (A[n] - A[n - 1]) / H[n - 1]
    return G


def periodic(G, A, H, n):
    G[n - 1][0] = 1
    G[n - 1][n] = -1

    G[n][0] = 2 * H[0]
    G[n][1] = H[0]
    G[n][n - 1] = H[n - 1]
    G[n][n] = 2 * H[n - 1]
    G[n][n + 1] = 3 * (A[1] - A[0]) / H[0] - 3 * (A[n] - A[n - 1]) / H[n - 1]
    return G


def fourth(G, A, H, n):
    G[n - 1][0] = H[1]
    G[n - 1][1] = -H[0] - H[1]
    G[n - 1][2] = H[0]

    G[n][n - 2] = H[n - 1]
    G[n][n - 1] = -H[n - 2] - H[n - 1]
    G[n][n] = H[n - 2]
    return G


def Spline(n):
    H, A, B, C, D = [], [], [], [], []
    for i in range(0, n):
        A.append(Y[i])
        H.append(X[i + 1] - X[i])
    A.append(Y[n])
    G = [[0] * (n + 2) for i in range(n + 1)]
    for j in range(1, n):
        G[j - 1][j - 1] = H[j - 1]
        G[j - 1][j] = 2 * (H[j - 1] + H[j])
        G[j - 1][j + 1] = H[j]
        G[j - 1][n + 1] = 3 * (A[j + 1] - A[j]) / H[j] - 3 * (A[j] - A[j - 1]) / H[j - 1]
    spline(A, dco(B), C, dco(D), H, natural(dco(G), A, H, n), n, "Natural Boundary", "blue", 2.0, "dotted")
    spline(A, dco(B), C, dco(D), H, clamped(dco(G), A, H, n), n, "Clamped Boundary", "orange", 2.0, "--")
    spline(A, dco(B), C, dco(D), H, periodic(dco(G), A, H, n), n, "Periodic Boundary", "green", 2.0, "-.")
    spline(A, dco(B), C, dco(D), H, fourth(dco(G), A, H, n), n, "Fourth Boundary", "red", 2.0, ":")


def main():
    n = 11
    X.append(l)
    X.append(r)
    for i in range(0, n - 2):
        X.append(random.uniform(l, r))
    X.sort()
    for i in range(0, n):
        Y.append(func(X[i]))
    # Draw Point
    plt.scatter(X, Y, s=50, label="Key Point")
    Spline(n - 1)

    # draw the original
    x = np.linspace(l, r, 100)
    y = np.sin(x)
    plt.plot(x, y, color="purple", linewidth=2.0, linestyle="-",label="y=sin(x)")
    plt.legend(loc="upper right")

    plt.tight_layout()
    plt.savefig('plot3-4.pdf',dpi = 700)
    plt.show()


if __name__ == "__main__":
    main()
