#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

void proceedLCP(const std::string_view str, std::vector<size_t>& suf_arr, std::vector<size_t>& result) {
    size_t str_len = str.length();
    int min_flag = 0;
    std::vector<size_t> positions(str_len);
    
    for (size_t i = 0; i < str_len; i++) 
        positions[suf_arr[i] - 1] = i;
    for (size_t i = 0; i < str_len; i++) {
        if (min_flag > 0) min_flag--;
        if (positions[i] == str_len - 1) {
            result[str_len - 1] = -1;
            min_flag = 0;
        } else {
            size_t temp = suf_arr[positions[i] + 1] - 1;
            size_t maxi = std::max(i + min_flag, temp + min_flag);
            while (maxi < str_len && str[i + min_flag] == str[temp + min_flag]) {
                min_flag += 1;
                maxi = std::max(i + min_flag, temp + min_flag);
            }
            
            result[positions[i]] = min_flag;
        }
    }
}

int main() {
    int N = 0;
    std::string str;
    std::vector<size_t> suf_arr;

    std::cin >> N;
    std::cin >> str;

    for (int i = 0; i < N; i++) {
        int temp = 0;
        std::cin >> temp;
        suf_arr.push_back(temp);
    }

    std::vector<size_t> res(N);
    proceedLCP(str, suf_arr, res);

    for (size_t i = 0; i < N - 1; i++) {
        std::cout << res[i] << ' ';
    }
    std::cout << '\n';
    
    return 0;
}