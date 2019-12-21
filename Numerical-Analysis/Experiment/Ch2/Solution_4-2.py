import matplotlib.pyplot as plt
import numpy as np
import matplotlib
from matplotlib import style

style.use('ggplot')
matplotlib.rcParams['text.usetex'] = True
plt.figure(figsize=(10, 10), dpi=700)
f, ax = plt.subplots(2, 2)
style_list = ["g+-", "r--", "b.-", "yo-"]


def func(x):
    return 5 * x - np.exp(x)


def func2(x):
    return x - np.exp(x)


def solve1(l, r):
    # X, Y coordinate point
    X = []
    Y = []
    tot = 0

    # Calculation process
    while abs(r - l) > 1e-5:
        mid = (l + r) / 2
        tot = tot + 1
        if func(mid) > 0:
            r = mid
        else:
            l = mid
        X.append(tot)
        Y.append(mid)
        if tot >= 200:
            break

    ax[0][0].plot(X, Y, '-p', color='grey',
                  marker='o',
                  markersize=8, linewidth=2,
                  markerfacecolor='red',
                  markeredgecolor='grey', markeredgewidth=2)
    ax[0][0].set_title("Bisection Method", fontsize=12)


def solve2(x0):
    # X, Y coordinate point
    X = []
    Y = []
    tot = 1
    X.append(0)
    Y.append(x0)
    x1 = x0 - func(x0) / func2(x0)
    X.append(1)
    Y.append(x1)

    # Calculation process
    while abs(x0 - x1) > 1e-5:
        tot = tot + 1
        x0 = x1
        x1 = x0 - func(x0) / func2(x0)
        X.append(tot)
        Y.append(x1)
        if tot >= 200:
            break

    ax[0][1].plot(X, Y, '-p', color='grey',
                  marker='o',
                  markersize=8, linewidth=2,
                  markerfacecolor='red',
                  markeredgecolor='grey', markeredgewidth=2)
    ax[0][1].set_title("Newton's Method", fontsize=12)


def solve3(x0, x1):
    # X, Y coordinate point
    X = []
    Y = []
    tot = 2
    X.append(0)
    Y.append(x0)
    X.append(1)
    Y.append(x1)

    # Calculation process
    while abs(x0 - x1) > 1e-5:
        tot = tot + 1
        x = x1 - func(x1) * (x1 - x0) / (func(x1) - func(x0))
        X.append(tot)
        Y.append(x)
        x0 = x1
        x1 = x
        if tot >= 200:
            break

    ax[1][0].plot(X, Y, '-p', color='grey',
                  marker='o',
                  markersize=8, linewidth=2,
                  markerfacecolor='red',
                  markeredgecolor='grey', markeredgewidth=2)
    ax[1][0].set_title("Secant Method", fontsize=12)


def solve4(x0, x1):
    # X, Y coordinate point
    X = []
    Y = []
    tot = 2
    X.append(0)
    Y.append(x0)
    X.append(1)
    Y.append(x1)

    # Calculation process
    while abs(x0 - x1) > 1e-5:
        tot = tot + 1
        x = x1 - func(x1) * (x1 - x0) / (func(x1) - func(x0))
        X.append(tot)
        Y.append(x)
        if func(x) * func(x1) < 0:
            x0 = x
        else:
            x1 = x
        if tot >= 200:
            break

    ax[1][1].plot(X, Y, '-p', color='grey',
                  marker='o',
                  markersize=8, linewidth=2,
                  markerfacecolor='red',
                  markeredgecolor='grey', markeredgewidth=2)
    ax[1][1].set_title("False Position Method", fontsize=12)


def main():
    solve1(0, 1)
    solve2(0.5)
    solve3(0, 1)
    solve4(0, 1)
    plt.tight_layout()
    plt.savefig('plot4-2.pdf', dpi=700)
    plt.show()


if __name__ == '__main__':
    main()
