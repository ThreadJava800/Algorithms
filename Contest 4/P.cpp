    #include <iostream>
    #include <vector>
    #include <climits>

    static const int64_t INF = __INT64_MAX__ / 100;

    int64_t shortestPath(std::vector<std::vector<int64_t>>& dist);
    void backTrace(std::vector<std::vector<int64_t>>& dist);
    inline bool contains(int64_t mask, int64_t v);

    inline bool contains(int64_t mask, int64_t v) {
        return mask & (1LL << v);
    }

    std::vector<std::vector<int64_t>> dp;
    std::vector<int64_t> prev;

int64_t shortestPath(std::vector<std::vector<int64_t>>& dist) {
    int64_t maxMask = (1 << dist.size()) - 1;

    int64_t listSize = dist.size();
    for (int mask = 0; mask <= maxMask; mask++) {
        
        for (int i = 0; i < listSize; ++i) {

            for (int j = 0; j < listSize; ++j) {

                if (mask & (1 << j)) {
                    dp[i][mask] = std::min(dp[i][mask], dp[j][mask - (1 << j)] + dist[i][j]);
                }
            }
        }
    }

    return dp[0][maxMask];
}

    void backTrace(std::vector<std::vector<int64_t>>& dist) {
        int64_t ind  = 0;
        int64_t mask = (1LL << dist.size()) - 1LL;

        while (mask > 0)
        {
            for (int64_t j = 0; j < dist.size(); j++) {
                if (contains(mask, j) && dp[ind][mask] == dp[j][mask ^ (1LL << j)] + dist[ind][j]) {
                    if (j > 0) prev.push_back(j);
                    ind = j;
                    mask -= (1LL << j);
                }
            }
        }
        
    }

    int main() {
        int64_t n = 0;
        std::cin >> n;

        std::vector<std::vector<int64_t>> dist(n + 1LL, std::vector<int64_t>(n + 1LL, 0));

        for (int64_t i = 1LL; i <= n; i++) {
            for (int64_t j = 1LL; j <= n; j++) {
                std::cin >> dist[i][j];
            }
        }

        dp = std::vector<std::vector<int64_t>>(dist.size(), std::vector<int64_t>(1LL << dist.size(), INF));
        dp[0][0] = 0;

        int64_t res = shortestPath(dist);

        std::cout << res << '\n';
        backTrace(dist);

        for (int64_t i = prev.size() - 1; i >= 0; i--)
            std::cout << prev[i] << ' ';

        return 0;
    }