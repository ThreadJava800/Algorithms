#include <iostream>
#include <vector>
#include <algorithm>

static const long MIN_INF = long(-2 * 1e31);
static const long PLU_INF = long( 2 * 1e31);

size_t mBinSearch(std::vector<long>& values, size_t l, size_t r, long cmpElem);

size_t mBinSearch(std::vector<long>& values, size_t l, size_t r, long cmpElem) {
    size_t resultIndex = 0;

    while (l <= r)
    {
        size_t mid = l + (r - l) / 2;
        
        if (values[mid] == cmpElem) return mid;
        if (values[mid] < cmpElem)  l = mid + 1;
        else {
            resultIndex = mid;
            r = mid - 1;
        }
    }

    return resultIndex;
}

int main() {
    size_t n = 0, len = 0;
    std::cin >> n;

    std::vector<long> values(n);
    for (size_t i = 0; i < n; i++)
        std::cin >> values[i];

    std::reverse(std::begin(values), std::end(values));

    std::vector<long> d   (n);
    std::vector<long> pos (n); 
    std::vector<long> prev(n - 1);

    d  [0] = MIN_INF;
    pos[0] = -1;

    for (size_t i = 1; i < n; i++)
        d[i] = PLU_INF;

    for (size_t i = 0; i < n - 1; i++) {
        size_t j = mBinSearch(d, 0, n - 1, values[i]);

        if (d[j - 1] < values[i] && values[i] < d[j]) {
            d   [j] = values[i];
            pos [j] = long(i);
            prev[i] = pos[j - 1];
            len     = std::max(len, j);
        }
    }

    std::vector<long> result;
    long p = pos[len];
    while (p != -1)
    {
        result.push_back(long(n - size_t(p)));
        p = prev[size_t(p)];
    }

    std::cout << result.size() << '\n';
    for (size_t i = 0; i < result.size(); i++) {
        std::cout << result[i] << ' ';
    }
    std::cout << '\n';
    

    return 0;
}