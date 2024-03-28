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
    
    int symbol = 0;
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

int main() {
    size_t N = 0;
    std::vector<int> arr;
    std::cin >> N;

    for (size_t i = 0; i < N; i++) {
        size_t temp = 0;
        std::cin >> temp;
        arr.push_back(temp);
    }

    std::string res = buildFromPrefix(arr);
    std::cout << res << '\n';

    return 0;
}