import math
import matplotlib.pyplot as plt
import numpy as np

from matplotlib import style

style.use('ggplot')

plt.figure(figsize=(10, 10), dpi=80)
f, ax = plt.subplots(2, 2)
style_list = ["g+-", "r--", "b.-", "yo-"]


def fz(u, y, z):
    return u * (1 - y * y) * z - y


def fy(z):
    return z


def RungeKutta(a, b, n, y0, z0, u, id1, id2):
    X, Y, Z = [], [], []
    X.append(0)
    Y.append(y0)
    Z.append(z0)
    h = (b - a) / n
    for i in range(1, n + 1):
        K11 = h * fy(Z[i - 1])
        K21 = h * fz(u, Y[i - 1], Z[i - 1])

        K12 = h * fy(Z[i - 1] + K21 / 2)
        K22 = h * fz(u, Y[i - 1] + K11 / 2, Z[i - 1] + K21 / 2)

        K13 = h * fy(Z[i - 1] + K22 / 2)
        K23 = h * fz(u, Y[i - 1] + K12 / 2, Z[i - 1] + K22 / 2)

        K14 = h * fy(Z[i - 1] + K23)
        K24 = h * fz(u, Y[i - 1] + K13, Z[i - 1] + K23)

        Y.append(Y[i - 1] + (K11 + 2 * K12 + 2 * K13 + K14) / 6)
        Z.append(Z[i - 1] + (K21 + 2 * K22 + 2 * K23 + K24) / 6)
        X.append(a + i * h)
        print(X[i], Y[i])

    ax[id1][id2].plot(X, Y, '-p', color='red',
                      marker='o',
                      markersize=0.1, linewidth=0.1,
                      markerfacecolor='red',
                      markeredgecolor='red',
                      markeredgewidth=2)


def main():
    l, r = 0, 25
    RungeKutta(l, r, 2500, 1, 0, 0.01, 0, 0)
    RungeKutta(l, r, 2500, 1, 0, 0.05, 0, 1)
    RungeKutta(l, r, 2500, 1, 0, 0.1, 1, 0)
    RungeKutta(l, r, 2500, 1, 0, 1, 1, 1)
    ax[0][0].set_title(r"$\mu$=0.01", fontsize=12)
    ax[0][1].set_title(r"$\mu$=0.05", fontsize=12)
    ax[1][0].set_title(r"$\mu$=0.1", fontsize=12)
    ax[1][1].set_title(r"$\mu$=1", fontsize=12)

    plt.tight_layout()
    plt.savefig('plot5-5.pdf', dpi=700)
    plt.show()


if __name__ == "__main__":
    main()
