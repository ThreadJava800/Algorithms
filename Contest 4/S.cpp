#include <iostream>
#include <vector>

int main() {
    int M = 0, N = 0;
    std::cin >> M >> N;

    std::vector<std::vector<bool>> dp(1 << M, std::vector<bool>(1 << N, true));
    for (int i = 1; i < (1 << M); i++) {
        for (int j = 1; j < (1 << N); j++) {
            if (!(dp[i - 1][j - 1] && dp[i - 1][j] && dp[i][j - 1]))
                dp[i][j] = true;
            else dp[i][j] = false;
        }
    }

    for (int i = 0; i < (1 << M); i++) {
        for (int j = 0; j < (1 << N); j++) {
            std::cout << dp[i][j] << ' ';
        }
        std::cout << '\n';
    }

    std::vector<std::vector<int>> a(1 << M, std::vector<int>(1 << N, 0));
    a[0][0] = 2;

    for (int k = 1; k < N; k++) {
        for (int i = 0; i < (1 << N); i++) {
            for (int j = 1; j < (1 << M); j++) {
                a[k][i] += a[k - 1][j] * dp[j][i] + a[k - 1][j - 1] * dp[j][i] + a[k][j - 1] * dp[j][i];
            }
        }
    }
std::cout << '\n';
    for (int i = 0; i < (1 << M); i++) {
        for (int j = 0; j < (1 << N); j++) {
            std::cout << a[i][j] << ' ';
        }
        std::cout << '\n';
    }

    // int res = 0;
    // for (int i = 0; i < (1 << M); i++) {
    //     if () {

    //     }
    // }

    return 0;
}