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

    # Function drawing
    x = np.linspace(0.2, 0.6, 1000)
    y = 5 * x - np.exp(x)
    ax[0][0].plot(x, y, color="#800080", linewidth=1.0, linestyle="--")

    # Calculation process
    while abs(r - l) > 1e-5:
        mid = (l + r) / 2
        tot = tot + 1
        if func(mid) > 0:
            r = mid
        else:
            l = mid
        X.append(mid)
        Y.append(func(mid))
        if tot >= 200:
            break

    ax[0][0].scatter(X, Y, s=30)
    ax[0][0].set_title("Bisection Method", fontsize=12)


def solve2(x0):
    # X, Y coordinate point
    X = []
    Y = []
    tot = 1
    X.append(x0)
    Y.append(func(x0))
    x1 = x0 - func(x0) / func2(x0)
    X.append(x1)
    Y.append(func(x1))

    # Function drawing
    x = np.linspace(0, 3, 1000)
    y = 5 * x - np.exp(x)
    ax[0][1].plot(x, y, color="#800080", linewidth=1.0, linestyle="--")

    # Calculation process
    while abs(x0 - x1) > 1e-5:
        tot = tot + 1
        x0 = x1
        x1 = x0 - func(x0) / func2(x0)
        X.append(x1)
        Y.append(func(x1))
        if tot >= 200:
            break

    ax[0][1].scatter(X, Y, s=30)
    ax[0][1].set_title("Newton's Method", fontsize=12)


def solve3(x0, x1):
    # X, Y coordinate point
    X = []
    Y = []
    tot = 2
    X.append(x0)
    Y.append(func(x0))
    X.append(x1)
    Y.append(func(x1))

    # Function drawing
    xline = np.linspace(0, 1, 1000)
    yline = 5 * xline - np.exp(xline)
    ax[1][0].plot(xline, yline, color="#800080", linewidth=1.0, linestyle="--")

    # Calculation process
    while abs(x0 - x1) > 1e-5:
        tot = tot + 1
        x = x1 - func(x1) * (x1 - x0) / (func(x1) - func(x0))
        X.append(x)
        Y.append(func(x))
        x0 = x1
        x1 = x
        if tot >= 200:
            break

    ax[1][0].scatter(X, Y, s=30)
    ax[1][0].set_title("Secant Method", fontsize=12)


def solve4(x0, x1):
    # X, Y coordinate point
    X = []
    Y = []
    tot = 2
    X.append(x0)
    Y.append(func(x0))
    X.append(x1)
    Y.append(func(x1))

    # Function drawing
    xline = np.linspace(0, 1, 1000)
    yline = 5 * xline - np.exp(xline)
    ax[1][1].plot(xline, yline, color="#800080", linewidth=1.0, linestyle="--")

    # Calculation process
    while abs(x0 - x1) > 1e-5:
        tot = tot + 1
        x = x1 - func(x1) * (x1 - x0) / (func(x1) - func(x0))
        X.append(x)
        Y.append(func(x))
        if func(x) * func(x1) < 0:
            x0 = x
        else:
            x1 = x
        if tot >= 200:
            break

    ax[1][1].scatter(X, Y, s=30)
    ax[1][1].set_title("False Position Method", fontsize=12)


def main():
    solve1(0, 1)
    solve2(0.5)
    solve3(0, 1)
    solve4(0, 1)
    plt.tight_layout()
    plt.savefig('plot4-1.pdf', dpi=700)
    plt.show()


if __name__ == '__main__':
    main()
