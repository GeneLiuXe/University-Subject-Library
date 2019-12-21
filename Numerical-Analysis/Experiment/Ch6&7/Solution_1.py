def solve(A, n):
    L = [[0] * n for i in range(n)]
    U = [[0] * n for i in range(n)]
    for i in range(0, n):
        L[i][i] = 1
    for j in range(0, n):
        U[0][j] = A[0][j] / L[0][0]
        L[j][0] = A[j][0] / U[0][0]
    for i in range(1, n):
        for j in range(i, n):
            U[i][j] = A[i][j]
            for k in range(0, i):
                U[i][j] -= L[i][k] * U[k][j]
            U[i][j] /= L[i][i]

            L[j][i] = A[j][i]
            for k in range(0, i):
                L[j][i] -= L[j][k] * U[k][i]
            L[j][i] /= U[i][i]

    for i in range(0, n):
        for j in range(0, n):
            print(L[i][j], end=" \n"[j == n - 1])
    print("\n")
    for i in range(0, n):
        for j in range(0, n):
            print(U[i][j], end=" \n"[j == n - 1])
    print("\n")
    for i in range(0, n):
        for j in range(0, n):
            print(A[i][j], end=" \n"[j == n - 1])


def main():
    A = [[4, -1, 1],
         [4, -8, 1],
         [-2, 1, 5]]
    solve(A, 3)


if __name__ == '__main__':
    main()
