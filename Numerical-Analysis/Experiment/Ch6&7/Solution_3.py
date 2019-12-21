import math
import matplotlib.pyplot as plt
import numpy as np

from matplotlib import style

style.use('ggplot')

plt.figure(figsize=(10, 10), dpi=80)
f, ax = plt.subplots(1, 3)
style_list = ["g+-", "r--", "b.-", "yo-"]

import numpy as np


def NORM(A, B):
    tmp = 0
    for i in range(0, len(A)):
        tmp += (A[i] - B[i]) * (A[i] - B[i])
    return np.sqrt(tmp)


def solve(A, B, X0, n, N, TOL):
    X1 = []
    Y1 = []
    Z1 = []
    base = []
    base.append(0)
    X1.append(X0[0])
    Y1.append(X0[1])
    Z1.append(X0[2])
    X = [0] * n
    k = 1
    print(0, "&", '%.10f' % X0[0], "&", '%.10f' % X0[1], "&", '%.10f' % X0[2], "\\\\")
    while k <= N:
        for i in range(0, n):
            tmp = 0
            for j in range(0, i):
                tmp -= X[j] * A[i][j]
            for j in range(i + 1, n):
                tmp -= X0[j] * A[i][j]
            X[i] = (tmp + B[i]) / A[i][i]
        print(k, "&", end='')
        for i in range(0, n):
            print(" ", '%.10f' % X[i], " ", end="& "[i == n - 1])
        print("\\\\")
        base.append(k)
        X1.append(X[0])
        Y1.append(X[1])
        Z1.append(X[2])
        if NORM(X, X0) < TOL:
            print("Succeed! The number of iteration times is", k)
            break
        else:
            k = k + 1
            X0 = X[:]
    print("Fail!")
    ax[0].plot(base, X1, '-p', color='grey',
               marker='o',
               markersize=8, linewidth=2,
               markerfacecolor='red',
               markeredgecolor='grey',
               markeredgewidth=2)
    ax[0].set_title(r"$x_k$", fontsize=12)

    ax[1].plot(base, Y1, '-p', color='grey',
               marker='o',
               markersize=8, linewidth=2,
               markerfacecolor='red',
               markeredgecolor='grey',
               markeredgewidth=2)
    ax[1].set_title(r"$y_k$", fontsize=12)

    ax[2].plot(base, Z1, '-p', color='grey',
               marker='o',
               markersize=8, linewidth=2,
               markerfacecolor='red',
               markeredgecolor='grey',
               markeredgewidth=2)
    ax[2].set_title(r"$z_k$", fontsize=12)


def main():
    A = [[4, -1, 1],
         [4, -8, 1],
         [-2, 1, 5]]
    B = [7, -21, 15]
    # X0 = [-1, 1000, 500]
    X0 = [1,5,2]
    solve(A, B, X0, 3, 100, 0.0001)
    plt.tight_layout()
    # plt.savefig('plot6-4.pdf', dpi=700)
    plt.show()
    # for i in range(0, 3):
    #     print(Ans[i])
    # print(4 * Ans[0] - Ans[1] + Ans[2], 4 * Ans[0] - 8 * Ans[1] + Ans[2], -2 * Ans[0] + Ans[1] + 5 * Ans[2])


if __name__ == '__main__':
    main()
