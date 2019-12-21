import matplotlib.pyplot as plt
import numpy as np
import matplotlib
matplotlib.rcParams['text.usetex'] = True
from matplotlib import style
style.use('ggplot')


def Draw(x,r):
    x = np.array(x)
    r = np.array(r)
    result = x - r
    x_coordinates = np.array(range(0, 11))
    plt.scatter(x_coordinates, result, s=50)
    plt.ylim(-0.001, 0.007)
    plt.xlabel(r'n')
    plt.ylabel(r'$x_n-q_n$')
    plt.savefig('myplot1.png', dpi=700)
    plt.show()

def Solution():
    x = [0] * 11
    r = [0] * 11
    p = [0] * 11
    q = [0] * 11
    x[1] = 0.5
    r[0] = 0.994
    r[1] = r[0] / 2
    x[0] = p[0] = q[0] = 1
    p[1] = q[1] = 0.497
    for i in range(2, 11):
        x[i] = x[i - 1] / 2.0
        r[i] = r[i - 1] / 2
        p[i] = 3 * p[i - 1] / 2 - p[i - 2] / 2
        q[i] = 5 * q[i - 1] / 2 - q[i - 2]
    for i in range(0, 11):
        print(i, '%.10f' % x[i], '%.10f' % r[i], '%.10f' % p[i], '%.10f' % q[i])
    for i in range(0, 11):
        print(i, '%.10f' % (x[i] - r[i]), '%.10f' % (x[i] - p[i]),
              '%.10f' % (x[i] - q[i]))
    Draw(x,r)
    # Draw(x, p)
    # Draw(x, q)


Solution()
