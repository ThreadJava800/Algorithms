#include <iostream>
#include <string>
#include <vector>

void calculatePrefixFun (const std::string& str, std::vector<size_t>& pref_fun);
void    searchStrEntries(const std::string& str_s, const std::string& str_p, std::vector<size_t>& indexes);

void calculatePrefixFun(const std::string& str, std::vector<size_t>& pref_fun) {
    int str_len = static_cast<int>(str.length());

    int j = 0;
    for (int i = 1; i < str_len; i++) {
        j = pref_fun[i - 1];
        while (j > 0 && str[i] != str[j]) j = pref_fun[j - 1];

        if (str[i] == str[j]) j++;
        pref_fun[i] = j;
    }
}

void searchStrEntries(const std::string& str_s, const std::string& str_p, std::vector<size_t>& indexes) {
    size_t s_len = str_s.length(), p_len = str_p.length();

    std::string concatenated_strings = str_p + "#" + str_s;
    std::vector<size_t> pref_func(concatenated_strings.size(), 0);

    calculatePrefixFun(concatenated_strings, pref_func);

    for (size_t i = 0; i < s_len; i++) {
        if (pref_func[p_len + i + 1] == p_len) indexes.push_back(i - p_len + 1);
    }
}

int main() {
    std::string str_s, str_p;
    std::vector<size_t> indexes;
    std::cin >> str_s >> str_p;

    searchStrEntries(str_s, str_p, indexes);
    for (size_t val : indexes) std::cout << val << '\n';

    return 0;
}
