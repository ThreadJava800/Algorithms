#include <iostream>
#include <vector>

int main() {
    int n = 0, m = 0;
    std::cin >> n >> m;

    std::vector<int> cj(n, 0);
    std::vector<int> dj(m, 0);
    int** dp = new int*[n];
    for (int i = 0; i < n; i++) {
        dp[i] = new int[m];
        // for (int j = 0; j < m; j++) dp[i][j] = 0;
    }

    for (int i = 0; i < n; i++)
        std::cin >> cj[i];
    for (int i = 0; i < m; i++)
        std::cin >> dj[i];

    for (int i = 0; i < n; i++) {
        int bestVal = 0;

        for (int j = 0; j < m; j++) {
            if (i > 0)
                 dp[i][j] = dp[i - 1][j];
            else dp[i][j] = 0;

            if (cj[i] == dj[j] && dp[i][j] < bestVal + 1)
                dp[i][j] = bestVal + 1;
            if (cj[i] > dj[j] && dp[i][j] > bestVal)
                bestVal = dp[i][j];
        }
    }

    int maxPos = 0;
    int maxVal = 0;
    for (int i = 0; i < m; i++)
        maxVal = std::max(maxVal, dp[n - 1][i]);

    std::cout << maxVal << '\n';

    for (int i = 0; i < n; i++)
        delete[] dp[i];
    delete[] dp;

    return 0;
}