#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int N = 0;
    std::cin >> N;

    std::vector<int> distr(N, 0);
    for (int i = 0; i < N; i++)
        std::cin >> distr[i];

    std::vector<int> dpa  (N, 1), dpb  (N, 1);
    std::vector<int> prevA(N, 0), prevB(N, 0);

    int len = 1;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < i; j++) {
            if (distr[j] < distr[i] && dpa[i] < dpb[j] + 1) {
                dpa[i]   = dpb[j] + 1;
                prevA[i] = j;
            }
            if (distr[j] > distr[i] && dpb[i] < dpa[j] + 1) {
                dpb[i]   = dpa[j] + 1;
                prevB[i] = j;
            }
        }

        if (len < std::max(dpa[i], dpb[i]))
            len = std::max(dpa[i], dpb[i]);
    }

    std::cout << len << '\n';

    int maxi = 0, index = 0;
    for (int i = 0; i < N; i++) {
        int tmp = std::max(dpa[i], dpb[i]);

        if (tmp > maxi) {
            maxi = tmp;
            index = i;
        }
    }

    std::vector<int> row;
    int aInd = 0, bInd = 0;

    if (maxi == dpa[index]) aInd = 1;
    else                    bInd = 1;

    row.push_back(distr[index]); // доб большего

    for (int i = index - 1; i >= 0; i--) {
        if (maxi == 1) break;

        if (bInd != 0) {
            if (dpa[i] + 1 == maxi) {
                row.push_back(distr[i]);
                maxi = dpa[i];
                aInd = 1, bInd = 0;
            }
        } else if (aInd != 0) {
            if (dpb[i] + 1 == maxi) {
                row.push_back(distr[i]);
                maxi = dpb[i];
                aInd = 0, bInd = 1;
            }
        }
    }

    for (int i = row.size() - 1; i >= 0; i--) 
        std::cout << row[i] << ' ';

    return 0;
}