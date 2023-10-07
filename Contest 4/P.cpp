    #include <iostream>
    #include <vector>
    #include <limits>

    static const size_t INF = 14 * 1e6;

    size_t shortestPath(std::vector<std::vector<size_t>>& dist, std::vector<size_t>* prev);
    inline bool contains(size_t mask, size_t v);

    inline bool contains(size_t mask, size_t v) {
        return mask & (1 << v);
    }

    size_t shortestPath(std::vector<std::vector<size_t>>& dist, std::vector<size_t>* prev) {
        size_t all = (1 << dist.size()) - 1;
        std::vector<std::vector<size_t>> dp(dist.size(), std::vector<size_t>(1 << dist.size(), INF));  

        size_t listSize = dp.size();
        for (size_t i = 0; i < listSize; i++)
            dp[i][1 << i] = 0;

        for (size_t mask = 1; mask <= all; mask++) {
            for (size_t vert1 = 0; vert1 < listSize; vert1++) {

                if (contains(mask, vert1)) {
                    for (size_t vert2 = 0;  vert2 < listSize; vert2++) {

                        if (!contains(mask, vert2)) {

                            if (dp[vert2][mask | (1 << vert2)] > dp[vert1][mask] + dist[vert1][vert2]) {
                                dp[vert2][mask | (1 << vert2)] = dp[vert1][mask] + dist[vert1][vert2];
                                
                                (*prev)[mask | (1 << vert2)] = vert1;
                            }
                        }
                    }
                }
            }
        }

        size_t path = INF;
        for (size_t i = 0; i < listSize; i++) {
            path = std::min(path, dp[i][all]);
        }

        return path;
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

        std::vector<size_t> prev(n, 0);

        std::cout << shortestPath(dist, &prev) << '\n';
        for (size_t i = 0; i < prev.size(); i++)
            std::cout << prev[i] << ' ';

        return 0;
    }