import math
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import style

style.use('ggplot')
plt.figure(figsize=(10, 10), dpi=80)
style_list = ["g+-", "r--", "b.-", "yo-"]


def fz(u, y, z):
    return u * (1 - y * y) * z - y


def fy(z):
    return z


def RungeKutta(a, b, n, y0, z0, u, color, top):
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

    plt.plot(X, Y, '-p', color=color,
             marker='o',
             markersize=0.5, linewidth=0.5,
             markerfacecolor=color,
             markeredgecolor=color,
             label=r"$\mu$=" + top,
             markeredgewidth=2)


def main():
    l, r = 0, 50
    RungeKutta(l, r, 5000, 1, 0, 0.01, "lightcoral", "0.01")
    RungeKutta(l, r, 5000, 1, 0, 0.05, "goldenrod", "0.05")
    RungeKutta(l, r, 5000, 1, 0, 0.1, "mediumseagreen", "0.1")
    RungeKutta(l, r, 5000, 1, 0, 1, "mediumpurple", "1")

    plt.legend(loc="best")
    plt.tight_layout()
    plt.savefig('plot5-6.pdf', dpi=700)
    plt.show()


if __name__ == "__main__":
    main()
