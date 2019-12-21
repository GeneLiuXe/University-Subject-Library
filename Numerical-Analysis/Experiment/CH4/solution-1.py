import math
import matplotlib.pyplot as plt
import numpy as np

from matplotlib import style

style.use('ggplot')

plt.figure(figsize=(10, 10), dpi=80)
style_list = ["g+-", "r--", "b.-", "yo-"]


def func(x):
    return np.exp(-x * x / 2)


v = 0.3413447460685429


def trapezoidal(a, b, h):
    n, ans = (b - a) / h, func(a) + func(b)
    for i in range(1, int(n)):
        ans += 2 * func(a + i * h)
    ans *= h / 2
    ans /= np.sqrt(2 * np.pi)
    return ans


def simpson(a, b, h):
    n, ans = (b - a) / h, func(a) + func(b)
    for i in range(1, int(n / 2) - 1):
        ans += 2 * func(a + 2 * i * h) + 4 * func(a + (2 * i - 1) * h)
    ans += 4 * func(a + (2 * (n - 1) - 1) * h)
    ans *= h / 3
    ans /= np.sqrt(2 * np.pi)
    return ans


def draw(n):
    x = np.linspace(0, 1, 100)
    y = np.exp(-x * x / 2)
    plt.plot(x, y, color="blue", linewidth=2.0, linestyle="-", label=r"$e^{-x^2/2}$")
    plt.legend(loc="upper right")

    h = 1 / n
    for i in range(0, n + 1):
        x1 = h*i
        y1 = 0
        y2 = func(x1)
        plt.plot([x1, x1], [y1, y2], '-p', color='grey',
                     marker='o',
                     markersize=8, linewidth=2,
                     markerfacecolor='red',
                     markeredgecolor='grey',
                    markeredgewidth = 2)
        if i > 0:
            X1 = h*(i-1)
            X2 = h*i
            Y1 = func(X1)
            Y2 = func(X2)
            plt.plot([X1, X2], [Y1, Y2], '--', color='grey',
                     marker='o',
                     markersize=8, linewidth=2,
                     markerfacecolor='red',
                     markeredgecolor='grey',
                     markeredgewidth=2)

def main():
    h = 0
    for i in range(0, 5):
        h += 0.001
        ans1 = trapezoidal(0, 1, h)
        ans2 = simpson(0, 1, h)
        print(h, " & ", int(1 / h), " & ", '%.10f' % ans1, " & ", '%.10f' % abs(ans1 - v), " & ",
              '%.10f' % ans2,
              " & ", '%.10f' % abs(ans2 - v), "\\\\")
    draw(10)
    plt.tight_layout()
    plt.savefig('plot4-2.pdf', dpi=700)
    plt.show()

if __name__ == "__main__":
    main()
