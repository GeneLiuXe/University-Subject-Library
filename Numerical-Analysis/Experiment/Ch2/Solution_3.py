import math
import matplotlib.pyplot as plt
import numpy as np

from matplotlib import style

style.use('ggplot')

import matplotlib

matplotlib.rcParams['text.usetex'] = True

plt.figure(figsize=(10, 10), dpi=80)
f, ax = plt.subplots(3, 1)
style_list = ["g+-", "r--", "b.-", "yo-"]


def func(x):
    return (0.5 + 0.25 * x * x - x * math.sin(x) - 0.5 * math.cos(2 * x));


def func1(x):
    return (0.5 * x - math.sin(x) - x * math.cos(x) + math.sin(2 * x))


def solve(x0, id):
    # 函数绘制
    if id == 0:
        x = np.linspace(1.75, 1.95, 100)
        y = 0.5 + 0.25 * x * x - x * np.sin(x) - 0.5 * np.cos(2 * x)
        ax[id].plot(x, y, color="#800080", linewidth=1.0, linestyle="--", label=r"$x_0=\frac{\pi}{2}$")
    elif id == 1:
        x = np.linspace(0, 25, 100)
        y = 0.5 + 0.25 * x * x - x * np.sin(x) - 0.5 * np.cos(2 * x)
        ax[id].plot(x, y, color="#800080", linewidth=1.0, linestyle="--", label=r"$x_0=5\pi$")
    elif id == 2:
        x = np.linspace(-300000, 300000, 100)
        y = 0.5 + 0.25 * x * x - x * np.sin(x) - 0.5 * np.cos(2 * x)
        ax[id].plot(x, y, color="#800080", linewidth=1.0, linestyle="--", label=r"$x_0=10\pi$")

    ax[id].legend(loc="best")
    # 计算过程
    tot = 1
    x1 = x0 - func(x0) / func1(x0)

    ax[id].scatter([x1], [func(x1)], s=15, color="blue")
    while abs(x0 - x1) > 1e-5:
        tot = tot + 1
        x0 = x1
        x1 = x0 - func(x0) / func1(x0)
        ax[id].scatter([x1], [func(x1)], s=15, color="blue")
        # print(tot, ":(", x1, ",", func(x1), ")")
    ax[id].scatter([x1], [func(x1)], s=15, color="red")
    print("总迭代次数:", tot, ", 收敛点: (", x1, ",", func(x1), ")")


def main():
    solve(0.5 * math.pi, 0)
    solve(5 * math.pi, 1)
    solve(10 * math.pi, 2)
    plt.tight_layout()
    plt.savefig('plot2-41.pdf', dpi=700)
    plt.show()


if __name__ == '__main__':
    main()
