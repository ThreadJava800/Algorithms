#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <vector>

const size_t alphabet_len = 256;
const int MAX_LEN         = 1e5;

std::vector<int> createSufArr(std::string_view str) {
    size_t str_len = str.length(), eq_classes_cnt = 1;
    std::vector<int> perm(str_len), counter(std::max(str_len, alphabet_len), 0), res(str_len);
    std::vector<int> temp_perm(str_len), temp_res(str_len);

    for (size_t i = 0; i < str_len; i++)      ++counter[str[i]];
    for (size_t i = 1; i < alphabet_len; i++) counter[i] += counter[i - 1];
    for (size_t i = 0; i < str_len; i++)      perm[--counter[str[i]]] = i;

    res[perm[0]] = 0;

    for (size_t i = 1; i < str_len; i++) {
        if (str[perm[i]] != str[perm[i - 1]]) ++eq_classes_cnt;
        res[perm[i]] = eq_classes_cnt - 1;
    }

    for (size_t i = 0; (1 << i) < str_len; i++) {
        for (size_t j = 0; j < str_len; j++) {
            temp_perm[j] = perm[j] - (1 << i);
            if (temp_perm[j] < 0) temp_perm[j] += str_len;
        }
        for (size_t j = 0; j < eq_classes_cnt; j++) counter[j] = 0;
        for (size_t j = 0; j < str_len; j++)        ++counter[res[temp_perm[j]]];
        for (size_t j = 1; j < eq_classes_cnt; j++) counter[j] += counter[j - 1];
        for (int j = str_len - 1; j >= 0; j--)      perm[--counter[res[temp_perm[j]]]] = temp_perm[j];

        temp_res[perm[0]] = 0; eq_classes_cnt = 1;

        for (size_t j = 1; j < str_len; j++) {
            if (res[perm[j]] != res[perm[j - 1]] || res[(perm[j] + (1 << i)) % str_len] != res[(perm[j - 1] + (1 << i)) % str_len])
                ++eq_classes_cnt;
            temp_res[perm[j]] = eq_classes_cnt - 1;
        }

        res.swap(temp_res);
    }

    return perm;
}

void buildSufArr(std::string& str) {
    str += static_cast<char>(31);
    std::vector<int> cyclic = createSufArr(str);
    cyclic.erase(cyclic.begin());

    for (size_t i = 0; i < cyclic.size(); i++) {
        std::cout << cyclic[i] + 1 << ' ';
    }
    std::cout << '\n';
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0); std::cout.tie(0);

    std::string str;
    std::getline(std::cin, str);

    buildSufArr(str);

    return 0;
}