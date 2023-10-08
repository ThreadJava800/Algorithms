#include <iostream>
#include <vector>
#include <climits>

static const int INF = INT_MAX;

int main() {
    int n = 0, m = 0;
    std::cin >> m >> n;

    std::vector<std::vector<int>> attempts(n + 1, std::vector<int>(m + 1, 0));
    for (int i = 1; i <= n; i++) attempts[i][1] = 1;
    for (int j = 1; j <= m; j++) attempts[1][j] = j;


    for (int i = 2; i <= n; i++) {
        for (int j = 2; j <= m; j++) {
            attempts[i][j] = INF;

            for (int h = 1; h <= j; h++) {
                int temp = std::max(attempts[i - 1][h - 1], attempts[i][j - h]) + 1;
                attempts[i][j] = std::min(attempts[i][j], temp);
            }
        }
    }

    std::cout << attempts[n][m] - 1 << '\n';

    return 0;
}