import math
import matplotlib.pyplot as plt
import numpy as np

from matplotlib import style

style.use('ggplot')

plt.figure(figsize=(10, 10), dpi=80)
f, ax = plt.subplots(2, 2)
style_list = ["g+-", "r--", "b.-", "yo-"]


def func(y):
    return 1 + y * y


def Euler(a, b, n, y0, id1, id2):
    X, Y = [], []
    X.append(0)
    Y.append(y0)
    h = (b - a) / n
    for i in range(1, n + 1):
        tmp = Y[i - 1] + h * func(Y[i - 1])
        Y.append(Y[i - 1] + h * (func(Y[i - 1]) + func(tmp)) / 2)
        X.append(a + i * h)
        print(X[i], Y[i])
    if id1 == 1 and id2 == 0:
        ax[id1][id2].plot(X, Y, '-p', color='grey',
                          marker='o',
                          markersize=7, linewidth=2,
                          markerfacecolor='red',
                          markeredgecolor='grey',
                          label="Euler",
                          markeredgewidth=1.5)
    elif id1 == 1 and id2 == 1:
        ax[id1][id2].plot(X, Y, '-p', color='grey',
                          marker='o',
                          markersize=5, linewidth=2,
                          markerfacecolor='red',
                          markeredgecolor='grey',
                          label="Euler",
                          markeredgewidth=1)
    else:
        ax[id1][id2].plot(X, Y, '-p', color='grey',
                          marker='o',
                          markersize=8, linewidth=2,
                          markerfacecolor='red',
                          markeredgecolor='grey',
                          label="Euler",
                          markeredgewidth=2)


def main():
    l, r = 0, 1.5
    Euler(l, r, 5, 0, 0, 0)
    Euler(l, r, 10, 0, 0, 1)
    Euler(l, r, 20, 0, 1, 0)
    Euler(l, r, 50, 0, 1, 1)
    ax[0][0].set_title("n=5", fontsize=12)
    ax[0][1].set_title("n=10", fontsize=12)
    ax[1][0].set_title("n=20", fontsize=12)
    ax[1][1].set_title("n=50", fontsize=12)

    x = np.linspace(l, r, 100)
    y = np.tan(x)
    ax[0][0].plot(x, y, color="blue", linewidth=1.0, linestyle="-", label="tan(x)")
    ax[0][0].legend(loc="best")
    ax[0][1].plot(x, y, color="blue", linewidth=1.0, linestyle="-", label="tan(x)")
    ax[0][1].legend(loc="best")
    ax[1][0].plot(x, y, color="blue", linewidth=1.0, linestyle="-", label="tan(x)")
    ax[1][0].legend(loc="best")
    ax[1][1].plot(x, y, color="blue", linewidth=1.0, linestyle="-", label="tan(x)")
    ax[1][1].legend(loc="best")

    plt.tight_layout()
    plt.savefig('plot5-2.pdf', dpi=700)
    plt.show()


if __name__ == "__main__":
    main()
