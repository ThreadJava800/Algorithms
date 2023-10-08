#include <iostream>
#include <vector>
#include <climits>

static const long long INF = LLONG_MAX;

int main() {
    long long n = 0, m = 0;
    std::cin >> m >> n;

    if (n == 0 && m == 1) {
        std::cout << 0 << '\n';
        return 0;
    }
    if (n == 0) {
        std::cout << -1 << '\n';
        return 0;
    }

    std::vector<long long> attempts(n + 1, 0);
    
    long long attemptCnt = 0;
    while (attempts[n] < m - 1)
    {
        for (int i = n; i > 0; i--) {
            attempts[i] = attempts[i] + attempts[i - 1] + 1;
        }

        attemptCnt++;
    }
    
    std::cout << attemptCnt << '\n';

    return 0;
}