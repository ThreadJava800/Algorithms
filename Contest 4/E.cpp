#include <iostream>
#include <vector>

static const size_t PLUS_INF = 1e9;

int main() {
    size_t n = 0, m = 0;
    std::cin >> n >> m;

    std::vector<size_t> coords(n);

    for (size_t i = 0; i < n; i++)
        std::cin >> coords[i];

    std::vector<size_t> now;
    std::vector<size_t> last;
    size_t**            old = new size_t*[n];
    size_t**            dp  = new size_t*[n];

    // расстояния до деревень слева для всех i
    for (size_t i = 1; i < n; i++) {
        for (size_t j = 0; j < i - 1; j++) {
            now[i] += abs(coords[i] - coords[j]);
        }
    }

    for (size_t i = 0; i < n; i++) {
        dp[i]  = new size_t[n];
        old[i] = new size_t[n];
    }

    // минимальные расстояния от деревень до ближайшего почтового отделения на отрезке
    for (size_t i = 1; i < n; i++) {
        for (size_t j = 0; j < n - 1; j++) {
            size_t summa = 0;
            for (size_t t = j + 1; t < i - 1; t++) {
                summa += std::min(abs(coords[i] - coords[t]), abs(coords[i] - coords[t]));
                dp[i][j] = summa;
            }
        }
    }

    for (size_t k = 1; k < m; k++) {
        last = now;
        for (size_t i = 0; i < n; i++) now[i] = PLUS_INF;
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < i - 1; j++) {
                if (now[i] > dp[i][j] + last[j]) {
                    old[i][k] = j;
                    now[i] = dp[i][j] + last[j];
                }
            }
        }
    }

    return 0;
}