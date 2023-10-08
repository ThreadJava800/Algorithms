#include <iostream>
#include <vector>
#include <climits>

static const long long INF = INT_MAX;

long long getMinMax(long long n, long long m) {
    if (m == 1 || m == 0) return m;
    if (n == 1)           return m;

    long long mini = INF;
    for (long long i = 1; i <= m; i++) {
        long long temp = std::max(getMinMax(n - 1, i - 1), getMinMax(n, m - i));
        mini = std::min(temp, mini);
    }

    return mini + 1;
}

int main() {
    long long n = 0, m = 0;
    std::cin >> m >> n;

    if (m == 0) {
        std::cout << -1;
        return 0;
    }

    // std::vector<std::vector<long long>> attempts(n + 1, std::vector<long long>(m + 1, -1));
    std::cout << getMinMax(n, m) - 1;

    return 0;
}