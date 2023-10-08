#include <iostream>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;

    std::vector<std::vector<bool>> network(n, std::vector<bool>(n, false));

    for (int i = 0; i < n; i++) {
        std::string inet;
        std::cin >> inet;

        for (int j = 0; j < n; j++) {
            if (inet[j] == 'Y') network[i][j] = true;
            else                network[i][j] = false;
        }
    }

    std::vector<int> mask   (1 << n, 0);
    std::vector<int> i2     (n, 1);
    for (int i = 1; i < n; i++) i2[i] = i2[i - 1] * 2;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < (1 << n); j++) {
            if (mask[j] != 0) {
                int freeCompTest = j + 1;
                int minPow       = n - 1;

                for (int m = n - 1; m > i; m--) {
                    if (freeCompTest >= i2[m]) freeCompTest -= i2[m];
                    else {
                        minPow = m;
                        break;
                    }
                }

                if (freeCompTest < i2[i]) {
                    freeCompTest = j + 1;

                    for (int m = minPow; m >= 0; m--) {
                        if (freeCompTest >= i2[m])   freeCompTest -= i2[m];
                        else if (network[i][m]) mask[j + i2[m] + i2[i]] = mask[j] + 1;
                    }
                }
            }
        }

        for (int j = 0; j < n; j++) {
            if (network[i][j]) mask[i2[j] + i2[i] - 1] = 1;
        }
    }

    // seraching max
    int maxi = -1;
    for (int i = 0; i < (1 << n); i++) {
        maxi = std::max(maxi, mask[i]);
    }
    std::cout << (maxi << 1);

    return 0;
}