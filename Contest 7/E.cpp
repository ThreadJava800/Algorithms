#include <string>
#include <vector>
#include <iostream>

std::string main_str, pattern;
std::vector<int> tab, separator;

void initTable() {
    int cur_pos = 2, cond = 0, this_len = pattern.length();
    tab[0] = -1; tab[1] = 0;

    while (cur_pos <= this_len) {
        if (pattern[cur_pos - 1] == pattern[cond]) {
            cond++; tab[cur_pos] = cond; cur_pos++;
        } else if (cond > 0) {
            cond = tab[cond];
        } else {
            tab[cur_pos] = 0;
            cur_pos++;
        }
    }
    
}

int knut() {
    int i = 0, tmp = 0, counter = 0, len = main_str.length();

    while (i + tmp < len) {
        if (main_str[i + tmp] == pattern[i]) i++;
        else {
            if (tab[i] > -1) {
                tmp += (i - tab[i]);
                separator[counter] = tmp; counter++;
                i = tab[i];
            } else {
                return -1;
            }
        }
    }
    
    return counter;
}

void solve() {
    int knut_res = knut();
    if (knut_res < 0) std::cout << "Yes\n";
    else {
        std::cout << "No\n";

        int counter = 0;
        for (int i = 0; i < knut_res; i++) {
            while (counter < separator[i]) {
                std::cout << main_str[counter];
                
                counter++;
            }
            std::cout << ' ';
        }

        int main_str_len = main_str.length();
        while (counter < main_str_len) {
            std::cout << main_str[counter];
                
            counter++;
        }
    }
}


int main() {
    std::cin >> pattern >> main_str;
    tab.resize(pattern.length() + main_str.length());
    separator.resize(pattern.length() + main_str.length());

    initTable();
    solve();

    return 0;
}