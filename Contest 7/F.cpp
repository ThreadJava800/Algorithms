#include <algorithm>
#include <iostream>
#include <cmath>
#include <vector>

void makeArr(const std::vector<int>& values, std::vector<int>& result) {
    // result.resize(values.size(), 0);
    int l = 0, r = 0;

    for (int i = 1; i < values.size(); i++) {
        if (i <= r)
            result[i] = std::max(std::min(result[r + l - i + 1], r - i + 1), 0);
        while ((result[i] + i) < (int)values.size() && i >= (1 + result[i]) && values[result[i] + i] == values[i - 1 - result[i]])
            result[i]++;

        if (result[i] + i > r + 1) {
            l = i - result[i];
            r = result[i] + i - 1;
        }
    }
}

void prResult(const std::vector<int>& result) {
    for (int i = result.size() - 1; i >= 0; i--) {
        if (i == result[i])
            std::cout << result.size() - result[i] << ' ';
    }
    std::cout << '\n';
}

int main() {
    int N = 0, M = 0;
    std::cin >> N >> M;

    std::vector<int> values(N), result(N, 0);
    for (size_t i = 0; i < N; i++) {
        int val = 0;
        std::cin >> val;
        values[i] = val;
    }
    makeArr(values, result);
    prResult(result);

    return 0;
}