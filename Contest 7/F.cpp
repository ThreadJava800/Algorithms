#include <iostream>
#include <math.h>
#include <vector>

// KUB = N, CUT = M
int main() {
    std::vector<int> arr1, arr2;
    int N = 0, M = 0, col_cnt = 0;
    std::cin >> N >> M;
    arr2.resize(N + 1);
    arr1.resize(N + 1);

    for (int i = 1; i <= N; i++) {
        int tmp = 0;
        std::cin >> tmp;
        arr1[i] = tmp;
    }

    int castedN = int(round(N / 2)) + 1;
    for (int i = 1; i <= castedN; i++) {
        bool flag = false;
        int r = i - 1;

        for (int j = 1; j <= r; j++) {
            if (arr1[i + j - 1] >= arr1[i - j]) {
                flag = true;
            }
        }
        if (flag) {
            col_cnt++;
            arr2[col_cnt] = N - r;
        }
    }

    for (int i = 1; i <= col_cnt; i++) {
        // for (int j = 0; j < N; j++) {
        //     // if (N - arr2[i] == j - 1) {
        //         std::cout << arr1[j] << ' ';
        //     // }
        // }
        std::cout << arr2[i] << ' ';
    }

    return 0;
}