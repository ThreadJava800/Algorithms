#include <iostream>
#include <vector>

void backTrace(std::vector<size_t>& weights, size_t** dp, size_t n, size_t m);

void backTrace(std::vector<size_t>& weights, size_t** dp, size_t n, size_t m) {
    if (dp[n][m] == 0) return;

    if (dp[n - 1][m] == dp[n][m]) backTrace(weights, dp, n - 1, m);
    else {
        backTrace(weights, dp, n - 1, m - weights[n - 1]);
        std::cout << n << '\n';
    }
}

int main() {
    size_t n = 0, m = 0;
    std::cin >> n >> m;

    std::vector<size_t> weight(n);
    std::vector<size_t> price (n);
    size_t** dp = new size_t*[n + 1];

    for (size_t i = 0; i < n; i++)
        std::cin >> weight[i];
    for (size_t i = 0; i < n; i++)
        std::cin >> price[i];

    for (size_t i = 0; i <= n; i++) {
        dp[i]    = new size_t[m + 1];
        dp[i][0] = 0;
    }
    for (size_t i = 0; i <= m; i++) {
        dp[0][i] = 0;
    }

    for (size_t i = 1; i <= n; i++) {
        for (size_t cap = 1; cap <= m; cap++) {
            if (weight[i - 1] <= cap)
                dp[i][cap] = std::max(dp[i - 1][cap], dp[i - 1][cap - weight[i - 1]] + price[i - 1]);
            else
                dp[i][cap] = dp[i - 1][cap];
        }
    }

    backTrace(weight, dp, n, m);

    for (size_t i = 0; i <= n; i++) {
        delete[] dp[i];
    }

    delete[] dp;

    return 0;
}