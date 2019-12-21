import matplotlib.pyplot as plt
import numpy as np
from matplotlib import style

style.use('ggplot')

import matplotlib

matplotlib.rcParams['text.usetex'] = True

plt.figure(figsize=(10, 10), dpi=700)
f, ax = plt.subplots(1, 3)
style_list = ["g+-", "r--", "b.-", "yo-"]

X = [[]]


def Draw(x, y, id):
    X.append([])
    for i in range(0, len(y)):
        X[id].append(y[i])
    ax[id].plot(x, y, '-p', color='grey',
                marker='o',
                markersize=8, linewidth=2,
                markerfacecolor='red',
                markeredgecolor='grey',
                markeredgewidth=2)
    ax[id].set_xlabel(r'k')
    ax[id].set_ylabel(r'$x_k$')


def solve1(x0):
    point = []
    base = []

    # 计算过程
    cnt = 2
    x1 = 15 - x0 * x0
    point.append(x0)
    base.append(0)
    point.append(x1)
    base.append(1)
    while abs(x1 - x0) < 200:
    # while abs(x1-x0) < 10000000000:
        x0 = x1
        x1 = 15 - x0 * x0
        point.append(x1)
        base.append(cnt)
        cnt = cnt + 1
    Draw(base, point, 0)
    ax[0].set_title(r'$x^{k+1}=15-x_k^2$', fontsize=12)


def solve2(x0):
    point = []
    base = []

    # 计算过程
    cnt = 2
    x1 = 15 / (2 * x0 + 1)
    point.append(x0)
    base.append(0)
    point.append(x1)
    base.append(1)
    while abs(x1 - x0) > 1e-2:
    # while cnt < 28:
        x0 = x1
        x1 = 15 / (2 * x0 + 1)
        point.append(x1)
        base.append(cnt)
        cnt = cnt + 1
    Draw(base, point, 1)
    ax[1].set_title(r'$x^{k+1}=\frac{15}{2x_k+1}$', fontsize=12)


def solve3(x0):
    # 计算过程
    point = []
    base = []
    x1 = x0 - (2 * x0 * x0 + x0 - 15) / (4 * x0 + 1)
    point.append(x0)
    base.append(0)
    point.append(x1)
    base.append(1)
    cnt = 2
    while abs(x1 - x0) > 1e-8:
    # while cnt < 28:
        x0 = x1
        x1 = x0 - (2 * x0 * x0 + x0 - 15) / (4 * x0 + 1)
        point.append(x1)
        base.append(cnt)
        cnt = cnt + 1
    Draw(base, point, 2)
    ax[2].set_title(r'$x^{k+1}=x_k-\frac{2x_k^2+x_k-15}{4x_k+1}$', fontsize=12)


def main():
    solve1(2.2)
    solve2(2.2)
    solve3(2.2)
    plt.tight_layout()
    plt.savefig('plot2-23.pdf', dpi=700)
    plt.show()
    # for i in range(0, 28):
    #     if (i < len(X[0])):
    #         print(i, " & ", '%.10f' % X[0][i], " & ", '%.10f' % X[1][i], " & ", '%.10f' % X[2][i], "\\\\")
    #     else:
    #         print(i, " & ", "-$\\infty$", " & ", '%.10f' % X[1][i], " & ", '%.10f' % X[2][i], "\\\\")


if __name__ == '__main__':
    main()
