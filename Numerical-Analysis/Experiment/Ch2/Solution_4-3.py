import numpy as np


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
    while 1:
        mid = (l + r) / 2
        tot = tot + 1
        if func(mid) > 0:
            r = mid
        else:
            l = mid
        X.append(tot)
        Y.append(mid)
        if tot >= 17:
            break
    return Y


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
    while 1:
        tot = tot + 1
        x0 = x1
        x1 = x0 - func(x0) / func2(x0)
        X.append(tot)
        Y.append(x1)
        if tot >= 17:
            break
    return Y


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
    while 1:
        tot = tot + 1
        x = x1 - func(x1) * (x1 - x0) / (func(x1) - func(x0))
        X.append(tot)
        Y.append(x)
        x0 = x1
        x1 = x
        if tot >= 17:
            break
    return Y


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
    while 1:
        tot = tot + 1
        x = x1 - func(x1) * (x1 - x0) / (func(x1) - func(x0))
        X.append(tot)
        Y.append(x)
        if func(x) * func(x1) < 0:
            x0 = x
        else:
            x1 = x
        if tot >= 17:
            break
    return Y


def main():
    X = [[]] * 5
    X[0] = solve1(0, 1)
    X[1] = solve2(0.5)
    X[2] = solve3(0, 1)
    X[3] = solve4(0, 1)
    for i in range(0, len(X[0])):
        print(i, "&", '%.4f'%X[0][i], "&", '%.4f'%X[1][i], "&", '%.4f'%X[2][i], "&", '%.4f'%X[3][i], "\\\\")


if __name__ == '__main__':
    main()
