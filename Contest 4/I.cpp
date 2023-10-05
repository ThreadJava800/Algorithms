#include <iostream>
#include <vector>

int main() {
    int N = 0;
    std::cin >> N;

    std::vector<int> distr(N, 0);
    for (int i = 0; i < N; i++)
        std::cin >> distr[i];

    std::vector<int> dpa  (N, 1), dpb  (N, 1);
    std::vector<int> prevA(N, 0), prevB(N, 0);

    int len = 1;
    for (int i = 1; i < N; i++) {
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

    // int i = prevB[N - 1];
    // bool type = false;
    // while (i >= 0) {
    //     std::cout << distr[i] << ' ';

    //     if (!type) i = prevA[i];
    //     else       i = prevB[i];

    //     type = !type;
    // }

    for (int i = 0; i < N; i++) {
        std::cout << prevA[i] << ' ' << prevB[i] << '\n';
    }

    return 0;
}