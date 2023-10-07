#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int N;
    std::cin >> N;

    std::vector<int> nums(N);
    for (int i = 0; i < N; i++) {
        std::cin >> nums[i];
    }

    std::vector<int> dpa(N, 1);
    std::vector<int> dpb(N, 1);

    for (int i = 1; i < N; i++) {
        for (int j = 0; j < i; j++) {
            if      (nums[i] > nums[j] && dpa[i] < dpb[j] + 1) dpa[i] = dpb[j] + 1;
            else if (nums[i] < nums[j] && dpb[i] < dpa[j] + 1) dpb[i] = dpa[j] + 1;
        }
    }

    int len = 1;

    for (int i = 0; i < N; i++) {
        int tmp = std::max(dpa[i], dpb[i]);
        if (tmp > len) {
            len = tmp;
        }
    }

    std::vector<int> row;
    int index = -1;

    for (int i = N - 1; i >= 0; i--) {
        if (std::max(dpa[i], dpb[i]) == len && (index == -1 || nums[i] != nums[index])) {
            row.push_back(nums[i]);
            index = i;

            len--;
        }
    }

    std::reverse(row.begin(), row.end());

    std::cout << row.size() << '\n';
    for (int i = 0; i < row.size(); i++) {
        std::cout << row[i] << " ";
    }

    return 0;
}
