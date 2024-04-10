#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

char giveNewSymbolFast(std::vector<int>& pref, std::string_view str, int ind) {
    int try_ind = pref[ind - 1];
    std::vector<bool> ban_ban_ban(27, true);
    while (try_ind != 0) {
        ban_ban_ban[str[try_ind] - 'a'] = false;
        try_ind = pref[try_ind - 1];
    }
    
    int symbol = 1;
    while (symbol < 26) {
        if (ban_ban_ban[symbol]) break;
        symbol++;
    }
    
    return symbol + 'a';
}

std::string buildFromPrefix(std::vector<int>& pref) {
    if (pref.size() == 0) return "";

    std::string res = "a";

    for (int i = 1; i < pref.size(); i++) {
        if (pref[i] == 0) {
            res += giveNewSymbolFast(pref, res, i);
        }
        else res += res[pref[i] - 1];
    }

    return res;
}

std::vector<int> googleTranslator(const std::vector<int>& original) {
    std::vector<int> result(original.size(), 0);

    for (size_t i = 1; i < original.size(); i++) {
        for (int j = original[i] - 1; j >= 0; j--) {
            if (!result[i + j]) result[i + j] = j + 1;
            else break;
        }
    }

    return result;
}

int main() {
    size_t N = 0;
    std::vector<int> z_func;
    std::cin >> N;

    for (size_t i = 0; i < N; i++) {
        int temp_val = 0;
        std::cin >> temp_val;

        z_func.push_back(temp_val);
    }

    std::vector<int> pref_func = googleTranslator(z_func);
    std::cout << buildFromPrefix(pref_func) << '\n';

    return 0;
}