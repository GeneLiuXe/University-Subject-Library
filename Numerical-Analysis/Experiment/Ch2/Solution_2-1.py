import math
import matplotlib.pyplot as plt
import numpy as np


from matplotlib import style

style.use('ggplot')

import matplotlib

matplotlib.rcParams['text.usetex'] = True

plt.figure(figsize=(10, 10), dpi=80)
ax = plt.subplot(111)
style_list = ["g+-", "r--", "b.-", "yo-"]

def func(x):
    return (2 - 3 * x - math.sin(x));


def Draw(x, y):
    ax.plot(x, y, '-p', color='grey',
                marker='o',
                markersize=8, linewidth=2,
                markerfacecolor='red',
                markeredgecolor='grey',
                markeredgewidth=2)
    ax.set_xlabel(r'n')
    ax.set_ylabel(r'$p_n$')


def solve(l, r):
    point = []
    base = []
    # # 函数绘制
    # x = np.linspace(0, 1, 100)
    # y = 2 - 3 * x - np.sin(x)
    # ax.plot(x, y, color="#800080", linewidth=1.0, linestyle="--", label="y=2-3*x-sin(x)")
    # plt.legend(loc="upper right")

    # 计算过程
    tot = 0
    while r - l > 0.0005:
        mid = (l + r) / 2
        tot = tot + 1
        if func(mid) > 0:
            l = mid
        else:
            r = mid
        point.append(mid)
        base.append(tot)
    Draw(base,point);
        # ax.scatter([mid], [func(mid)], s=15, color="blue")
        # print(tot, " & ", '%.10f'%l , " & ", '%.10f'%r, " & ", '%.10f'%mid, "& ", '%.10f'%func(mid), "\\\\")
        # if tot < 7:
        #     plt.annotate(str(tot),
        #                  xy=(mid, func(mid)),
        #                  fontsize=15,
        #                  xycoords='data')
    # ax.scatter([r], [func(r)], s=15, color="red")
    # print("总迭代次数:", tot, ", 收敛点: (", r, ",", func(r), ")")


def main():
    solve(0, 1)
    plt.tight_layout()
    plt.savefig('plot3.pdf', dpi=700)
    plt.show()


if __name__ == '__main__':
    main()
