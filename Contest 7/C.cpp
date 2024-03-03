#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

std::string build_from_z(const std::vector<size_t>& z_func) {
    if (z_func.empty()) return "";

    std::string res = "a";

    size_t pref_len = 0, cur_pos = 1, pref_pos = 0;
    std::vector<size_t> used;

    while (cur_pos < z_func.size()) {
        if (z_func[cur_pos] == 0 && pref_pos == 0) {
            char cur_letter = 'b';
            while (std::any_of(used.begin(), used.end(), [cur_letter, &res](size_t i) {return res[i] == cur_letter;})) {
                cur_letter++;
            }

            res += cur_letter; used.clear(); cur_pos++;
        }
        else {
            pref_len = z_func[cur_pos]; pref_pos = 0;
            
            while (pref_len > 0) {
                if (z_func[cur_pos] > pref_len) {
                    pref_len = z_func[cur_pos]; pref_pos = 0;
                    used.push_back(z_func[cur_pos]);
                }

                if (z_func[cur_pos] == pref_len) used.push_back(z_func[cur_pos]);

                res += res[pref_pos];
                pref_pos++; pref_len--; cur_pos++;
            }
            pref_pos = 0;
        }
    }
    

    return res;
}

int main() {
    size_t N = 0;
    std::vector<size_t> arr;
    std::cin >> N;

    for (size_t i = 0; i < N; i++) {
        size_t temp = 0;
        std::cin >> temp;
        arr.push_back(temp);
    }

    std::string res = build_from_z(arr);
    std::cout << res << '\n';

    return 0;
}