import math
import matplotlib.pyplot as plt
import numpy as np

from matplotlib import style

style.use('ggplot')

plt.figure(figsize=(10, 10), dpi=80)
f, ax = plt.subplots(3, 1)
style_list = ["g+-", "r--", "b.-", "yo-"]


def func(x):
    return (0.5 + 0.25 * x * x - x * math.sin(x) - 0.5 * math.cos(2 * x));


def func1(x):
    return (0.5 * x - math.sin(x) - x * math.cos(x) + math.sin(2 * x))


def solve(x0, id):
    base = []
    point = []
    # 函数绘制
    # if id == 0:
    #     x = np.linspace(1.75, 1.95, 100)
    #     y = 0.5 + 0.25 * x * x - x * np.sin(x) - 0.5 * np.cos(2 * x)
    #     ax[id].plot(x, y, color="#800080", linewidth=1.0, linestyle="--", label=r"$x_0=\frac{\pi}{2}$")
    # elif id == 1:
    #     x = np.linspace(0, 25, 100)
    #     y = 0.5 + 0.25 * x * x - x * np.sin(x) - 0.5 * np.cos(2 * x)
    #     ax[id].plot(x, y, color="#800080", linewidth=1.0, linestyle="--", label=r"$x_0=5\pi$")
    # elif id == 2:
    #     x = np.linspace(-300000, 300000, 100)
    #     y = 0.5 + 0.25 * x * x - x * np.sin(x) - 0.5 * np.cos(2 * x)
    #     ax[id].plot(x, y, color="#800080", linewidth=1.0, linestyle="--", label=r"$x_0=10\pi$")

    # ax[id].legend(loc="best")

    # 计算过程
    tot = 1
    x1 = x0 - func(x0) / func1(x0)
    point.append(x0)
    point.append(x1)
    base.append(0)
    base.append(1)
    # ax[id].scatter([x1], [func(x1)], s=15, color="blue")
    while abs(x0 - x1) > 1e-5:
        tot = tot + 1
        x0 = x1
        x1 = x0 - func(x0) / func1(x0)
        point.append(x1)
        base.append(tot)
        # ax[id].scatter([x1], [func(x1)], s=15, color="blue")
        # print(tot, ":(", x1, ",", func(x1), ")")
    # ax[id].scatter([x1], [func(x1)], s=15, color="red")
    siz = 8
    s2 = 2
    s3 = 2
    if id == 2:
        siz = 5
        s2 = 1.5
        s3 = 0.30
    if id == 0:
        ax[id].plot(base, point, '-p', color='grey',
                 marker = 'o',
                 markersize=siz, linewidth=s2,
                 markerfacecolor='red',
                 markeredgecolor='grey',
                 markeredgewidth=s3,
                 label=r"$x_0=\frac{\pi}{2}$")
        ax[id].legend(loc="best")
    elif id == 1:
        ax[id].plot(base, point, '-p', color='grey',
                    marker='o',
                    markersize=siz, linewidth=s2,
                    markerfacecolor='red',
                    markeredgecolor='grey',
                    markeredgewidth=s3,
                    label=r"$x_0=5\pi$")
        ax[id].legend(loc="best")
    else:
        ax[id].plot(base, point, '-p', color='grey',
                    marker='o',
                    markersize=siz, linewidth=s2,
                    markerfacecolor='red',
                    markeredgecolor='grey',
                    markeredgewidth=s3,
                    label=r"$x_0=10\pi$")
        ax[id].legend(loc="best")
    print("总迭代次数:", tot, ", 收敛点: (", x1, ",", func(x1), ")")


def main():
    solve(0.5 * math.pi, 0)
    solve(5 * math.pi, 1)
    solve(10 * math.pi, 2)
    plt.tight_layout()
    plt.savefig('plot2-43.pdf', dpi=700)
    plt.show()


if __name__ == '__main__':
    main()
