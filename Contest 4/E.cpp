#include <iostream>
#include <vector>

static const int PLUS_INF = 1e5;

int main() {
    int n = 0, m = 0;
    std::cin >> n >> m;

    std::vector<int> coords(n, 0);
    std::vector<int> d(n, 0), t(n, 0);
    int** dp = new int*[n];
    for (int i = 0; i < n; i++) {
        dp[i] = new int[n];
        for (int j = 0; j < n ;j++) {
            dp[i][j] = 0;
        }
    }

    for (int i = 0; i < n; i++)
        std::cin >> coords[i];

    for (int i = 0; i < n; i++) {
        int sum = 0;
        for (int j = 0; j <= i - 1; j++) {
            sum += coords[i] - coords[j];
        }
        d[i] = sum;
    }

    for (int k = 1; k < m; k++) {
        for (int i = 0; i < n; i++) {
            t[i] = PLUS_INF;

            int r = i;
            int summa = 0;

            for (int j = i - 1; j >= 0; j--) {
                while((r > j) && (coords[i] - coords[r - 1] < coords[r - 1] - coords[j])) {
                    r--;
                    summa = summa - (coords[r] - coords[j + 1]) + (coords[i] - coords[r]);
                }

                summa += (r - j - 1) * (coords[j + 1] - coords[j]);
                if (summa + d[j] < t[i]) {
                    t[i] = summa + d[j];
                    dp[i][k] = j;
                }
            }
        }
        d = t;
    }

    int min = PLUS_INF;
    int mm = 0;
    for (int i = 0; i < n; i++) {
        int summa = d[i];
        for (int j = i + 1; j < n; j++) summa += (coords[j] - coords[i]);
        if (summa < min) {
            min = summa;
            mm = i;
        }
    }

    std::cerr << min << '\n';

    int k = m - 1;
    while (k >= 0)
    {
        d[k] = mm;
        mm = dp[mm][k];
        k--;
    }
    
    for (int i = 0; i < m; i++) {
        if (d[i] != 1e5) std::cerr << coords[d[i]] << ' ';
    }
    

    return 0;
}