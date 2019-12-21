import numpy as np


def NORM(A, B):
    tmp = 0
    for i in range(0, len(A)):
        tmp += (A[i] - B[i]) * (A[i] - B[i])
    return np.sqrt(tmp)


def solve(A, B, X0, n, N, TOL):
    X = [0] * n
    k = 1
    while k <= N:
        for i in range(0, n):
            tmp = 0
            for j in range(0, i):
                tmp -= X[j] * A[i][j]
            for j in range(i + 1, n):
                tmp -= X0[j] * A[i][j]
            X[i] = (tmp + B[i]) / A[i][i]
        if NORM(X, X0) < TOL:
            print("Succeed!")
            return
        else:
            k = k + 1
            X0 = X[:]
    print("Fail!")


def main():
    A = [[4, -1, 1],
         [4, -8, 1],
         [-2, 1, 5]]
    B = [7, -21, 15]
    X0 = [1, 5, 2]
    solve(A, B, X0, 3, 100, 0.0001)


if __name__ == '__main__':
    main()
