    #include <iostream>
    #include <vector>
    #include <limits>

    static const size_t INF = 14 * 1e6;

    size_t shortestPath(size_t ind, size_t mask, std::vector<std::vector<size_t>>& dist);
    inline bool contains(size_t mask, size_t v);

    inline bool contains(size_t mask, size_t v) {
        return mask & (1 << v);
    }

    std::vector<std::vector<size_t>> dp;

    size_t shortestPath(size_t ind, size_t mask, std::vector<std::vector<size_t>>& dist) {
        if (dp[ind][mask] != INF) return dp[ind][mask];

        for (size_t i = 0; i < dist.size(); i++) {
            if (contains(mask, i)) {
                dp[ind][mask] = std::min(dp[ind][mask], shortestPath(i, mask - (1 << i), dist) + dist[ind][i]);
            }
        }
        return dp[ind][mask];
    }

    int main() {
        short n = 0;
        std::cin >> n;

        std::vector<std::vector<size_t>> dist(n, std::vector<size_t>(n, 0));
        for (short i = 0; i < n; i++) {
            for (short j = 0; j < n; j++) {
                std::cin >> dist[i][j];
            }
        }

        std::vector<size_t> prev;
        dp = std::vector<std::vector<size_t>>(dist.size(), std::vector<size_t>(1 << dist.size(), INF));
        dp[0][0] = 0;

        std::cout << shortestPath(0, (1 << n) - 1, dist) << '\n';

        size_t i = 0;
        size_t mask = (1 << n) - 1;
        prev.push_back(0);

        while(mask != 0) {
            for (size_t j = 0; j < n; j++) {
                if (contains(mask, j) && dp[i][mask] == dp[j][mask - (1 << j)] + dist[i][j]) {
                    prev.push_back(j);
                    i = j;
                    mask -= (1 << j);
                    continue;
                }
            }
        }

        for (size_t i = 0; i < prev.size(); i++)
            std::cout << prev[i] << ' ';

        return 0;
    }