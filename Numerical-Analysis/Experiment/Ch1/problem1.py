import math


def Solution(a, b, c):
    x1 = x2 = 0
    tmp = b * b - 4 * a * c
    if a == 0:
        x1 = x2 = -c / b
    elif tmp < 0:
        print("No solution.\n")
        return
    else:
        tmp = math.sqrt(tmp)
        if c == 0:
            x1 = (-b + tmp) / (2 * a)
            x2 = (-b - tmp) / (2 * a)
        elif b > 0:
            x1 = -2 * c / (b + tmp)
            x2 = (-b - tmp) / (2 * a)
        else:
            x1 = (-b + tmp) / (2 * a)
            x2 = -2 * c / (b - tmp)
    print("x1:", x1, ", x2:", x2)


Solution(1, -1000.001, 1)
Solution(1, -10000.0001, 1)
Solution(1, -100000.00001, 1)
Solution(1, -1000000.000001, 1)
