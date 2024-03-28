#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>

class Node;

const size_t alphabet_len = 256;
const size_t state_cnt    = 30000;
Node* root                = nullptr;

class Aho_Corasick {
public:

    int proceedBFS(std::queue<int>& bfs_queue) {
        while (bfs_queue.size()) {
            int cur_state = bfs_queue.front();
            bfs_queue.pop();

            for (int symb = 0; symb < alphabet_len; symb++) {
                if (transitions[cur_state][symb] != -1) {
                    int fail = failure[cur_state];
                    while (transitions[cur_state][symb] == -1) {
                        fail = failure[fail];
                    }
                    
                    fail = transitions[fail][symb];
                    failure[transitions[cur_state][symb]] = fail;

                    states[transitions[cur_state][symb]] |= states[fail];
                    bfs_queue.push(transitions[cur_state][symb]);
                }
            }
        }

        return state_cnt;
    }

    int getNextState(int cur, char edge) {
        int res = cur;
        
        while (transitions[res][edge] == -1) {
            res = failure[res];
        }
        
        return transitions[res][edge];
    }

    int init(std::vector<std::string>& strings) {
        std::memset(states, 0, sizeof states);
        std::memset(transitions, -1, sizeof transitions);
        std::memset(failure, -1, sizeof failure);

        int state_cnt = 1;
        for (size_t i = 0; i < strings.size(); i++) {
            std::string cur_word = strings[i];
            int cur_state_num = 0;

            for (size_t j = 0; j < cur_word.length(); j++) {
                char symb = cur_word[j];
                if (transitions[cur_state_num][symb] == -1) {
                    transitions[cur_state_num][symb] = state_cnt;
                    state_cnt++;
                }
                cur_state_num = transitions[cur_state_num][symb];
            }

            states[cur_state_num] |= (1 << i);
        }

        for (int symb = 0; symb < alphabet_len; symb++) {
            if (transitions[0][symb] == -1)
                transitions[0][symb] = 0;
        }

        std::queue<int> bfs_queue;
        for (int symb = 0; symb < alphabet_len; symb++) {
            if (transitions[0][symb] != 0) {
                failure[transitions[0][symb]] = 0;
                bfs_queue.push(transitions[0][symb]);
            }
        }

        return proceedBFS(bfs_queue);
    }

public:
    int states     [state_cnt];
    int failure    [state_cnt];
    int transitions[state_cnt][alphabet_len];
};

Aho_Corasick aho;
std::vector<std::pair<int, int>> banned_ind;

bool findOccur(std::vector<std::string>& strings, std::string& str) {
    int cur_state = 0;

    for (size_t i = 0; i < str.length(); i++) {
        cur_state = aho.getNextState(cur_state, str[i]);
        if (aho.states[cur_state] == 0) continue;

        for (size_t j = 0; j < strings.size(); j++) {
            if (aho.states[cur_state] & (1 << j)) {
                // std::cout << str << ' ' << << '\n';
                // std::cout << "OUT " << i - strings[j].size() + 1 << ' ' << i << '\n';
                if (std::find(banned_ind.begin(), banned_ind.end(), std::pair<int, int>(i - strings[j].size() +1, i)) != banned_ind.end()) {
                    continue;
                }
                return true;
            }
        }
    }

    return false;
}

int main() {
    

    return 0;
}