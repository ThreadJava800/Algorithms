#include <iostream>
#include <string>
#include <string_view>
#include <vector>

int calculatePrefix(const std::string_view& str) {
    std::vector<int> pref_arr(str.size(), 0);

    for (size_t i = 1; i < str.size(); i++) {
        int current_prefix_ind = pref_arr[i - 1];
        while (current_prefix_ind && str[i] != str[current_prefix_ind])
            current_prefix_ind = pref_arr[current_prefix_ind - 1];
        
        pref_arr[i] = str[i] == str[current_prefix_ind] ? current_prefix_ind + 1 : current_prefix_ind;
    }

    return pref_arr.back();
}

void splitCalambur(std::string& res_str, size_t N) {
    std::string help_str, hash_tag_str;

    for (size_t i = 1; i < N; i++) {
        std::cin >> help_str;
        if (res_str.size() > help_str.size()) {
            std::string hash_tag_part1(res_str.begin() + res_str.size() - help_str.size(), res_str.end());
            hash_tag_str = help_str + static_cast<char>(10) + hash_tag_part1;
        } else {
            hash_tag_str = help_str + static_cast<char>(10) + res_str;
        }

        res_str = res_str + help_str.substr(calculatePrefix(hash_tag_str));
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    size_t N = 0;
    std::string result_str;

    std::cin >> N;
    std::cin >> result_str;

    splitCalambur(result_str, N);
    std::cout << result_str << '\n';

    return 0;
}