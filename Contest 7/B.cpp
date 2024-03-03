#include <iostream>
#include <string>
#include <vector>

std::string buildFromPrefix(std::vector<int>& pref) {
    if (pref.empty()) return "";

    std::string res = "";
    char symbol = 'a' - 1;

    for (int i = 0; i < pref.size(); i++) {
        if (pref[i] == 0) {
            symbol++;
            res += symbol;
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