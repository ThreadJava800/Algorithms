#include <algorithm>
#include <iostream>
#include <vector>

int nod(int val1, int val2) {
    if (val1 < val2) std::swap(val1, val2);

    while (val1 % val2 != 0) {
        val1 %= val2;
        std::swap(val1, val2);
    }

    return val2;
}

int nod(std::vector<int>& values) {
    int res = values[0];

    size_t count = values.size();
    for (size_t i = 1; i < count; ++i) {
        res = nod(res, values[i]);
    }

    return res;
}

int getMax(std::vector<int>& values) {
    size_t count = values.size();

    int lcs = 1;
    int ind = 0;

    for (size_t i = 0; i < count - 1; ++i) {
        int t_lcs = nod(values[i], values[i + 1]);
        if (t_lcs >= lcs) {
            lcs = t_lcs;
            ind = i;
        }
    }

    int res = values[ind];
    values[ind] = lcs;
    return res;
}

int main() {
    size_t n = 0;
    std::cin >> n;

    std::vector<int> values(n);

    for (size_t i = 0; i < n; ++i) {
        std::cin >> values[i];
    }

    int maxi = 1;
    if (*std::max_element(values.begin(), values.end()) == 1) {
        std::cout << 0 << '\n';
        return 0;
    }

    int cnt = 0;
    while (maxi > 1)
    {
        int lcs = nod(values);
        if (lcs == 1) {
            break;
        }

        maxi = getMax(values);
        cnt++;
    }

    std::cout << cnt << '\n';
    

    return 0;
}