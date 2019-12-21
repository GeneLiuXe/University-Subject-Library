import matplotlib.pyplot as plt
import numpy as np
from matplotlib import style
style.use('ggplot')

import matplotlib
matplotlib.rcParams['text.usetex'] = True

plt.figure(figsize=(10, 10), dpi=700)
f, ax = plt.subplots(3, 1)
style_list = ["g+-", "r--", "b.-", "yo-"]


def solve1(x0):
    # 函数绘制
    x = np.linspace(-100.0, 100.5, 10)
    y1 = x
    y2 = 15 - x * x
    ax[0].plot(x, y1, color="blue", linewidth=1.0, linestyle="-")
    ax[0].plot(x, y2, color="#800080", linewidth=1.0, linestyle="--")

    # 计算过程
    x1 = 15 - x0 * x0
    ax[0].scatter([x0], [x1], s=15, color="blue")
    while abs(x1 - x0) < 200:
        x0 = x1
        x1 = 15 - x0 * x0
        ax[0].scatter([x0], [x1], s=15, color="blue")
    ax[0].scatter([x0], [x1], s=15, color="red")
    ax[0].set_title(r'$x^{k+1}=15-x_k^2$', fontsize = 12)

def solve2(x0):
    # 函数绘制
    x = np.linspace(2.0, 3.0, 10)
    y1 = x
    y2 = 15 / (2 * x + 1)
    ax[1].plot(x, y1, color="blue", linewidth=1.0, linestyle="-")
    ax[1].plot(x, y2, color="#800080", linewidth=1.0, linestyle="--")

    # 计算过程
    cnt = 0
    x1 = 15 / (2 * x0 + 1)
    ax[1].scatter([x0], [x1], s=15, color="blue")
    while abs(x1 - x0) > 1e-2:
        cnt = cnt + 1
        x0 = x1
        x1 = 15 / (2 * x0 + 1)
        ax[1].scatter([x0], [x1], s=15, color="blue")
    ax[1].scatter([x0], [x1], s=15, color="red")
    ax[1].set_title(r'$x^{k+1}=\frac{15}{2x_k+1}$',fontsize = 12)

def solve3(x0):
    # 函数绘制
    x = np.linspace(2.0, 5, 10)
    y1 = x
    y2 = x - (2 * x * x + x - 15) / (4 * x + 1)
    ax[2].plot(x, y1, color="blue", linewidth=1.0, linestyle="-")
    ax[2].plot(x, y2, color="#800080", linewidth=1.0, linestyle="--")

    # 计算过程
    x1 = x0 - (2 * x0 * x0 + x0 - 15) / (4 * x0 + 1)
    ax[2].scatter([x0], [x1], s=15, color="blue")
    while abs(x1 - x0) > 1e-8:
        x0 = x1
        x1 = x0 - (2 * x0 * x0 + x0 - 15) / (4 * x0 + 1)
        ax[2].scatter([x0], [x1], s=15, color="blue")
    ax[2].scatter([x0], [x1], s=15, color="red")
    ax[2].set_title(r'$x^{k+1}=x_k-\frac{2x_k^2+x_k-15}{4x_k+1}$',fontsize = 12)


def main():
    solve1(2)
    solve2(2)
    solve3(2)
    plt.tight_layout()
    plt.savefig('plot2-1.pdf',dpi = 700)
    plt.show()


if __name__ == '__main__':
    main()
