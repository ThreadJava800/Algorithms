#include <iostream>
#include <string>
#include <string_view>
#include <memory>
#include <vector>
#include <queue>
#include <map>
#include <cstring>
#include <algorithm>

const size_t alphabet_len = 256;

struct AhoNode {
public:

    AhoNode(int _parent = -1, char _edge = '\0')
      : children    (alphabet_len, -1),
        transitions (alphabet_len, -1),
        is_leaf     (false),
        parent      (_parent),
        edge        (_edge),
        suf_link    (-1) {
      }

public:
    std::vector<int> str_indexes;
    std::vector<int> children;
    std::vector<int> transitions;

    bool is_leaf;
    int  parent;
    char edge;
    int  suf_link;

    int  color = 0;
};

class Aho_Corasick {
public:

    Aho_Corasick() {
        nodes.push_back(new AhoNode);
    }

    void findMaskOccur(std::string_view mask_str, char mask_symb, std::string_view substr, std::vector<int>& result) {
        std::vector<size_t> start_positions;
        std::vector<size_t> substr_occurs(substr.size(), 0);

        splitToSubstrings(mask_str, mask_symb, start_positions);
        // init();

        // std::cout << substrings.size() << '\n';
        // int cnt = 0;
        // for (auto val : substrings) {
        //     std::cout << val << start_positions[cnt++] << '\n';
        // }

        findOccur(substr, start_positions, substr_occurs);

        for (size_t i = 0; i < substr_occurs.size(); i++) {
            // std::cout << i << ' ' << substr_occurs[i] << '\n';
            // std::cout << substrings.size() 
            if (substr_occurs[i] == substrings.size()) {
                result.push_back(i);
            }
        }
    }

public:

    void findOccur(const std::string_view str, std::vector<size_t>& start_positions, std::vector<size_t>& substr_occurs) {
        int cur = 0;

        for (size_t i = 0; i < str.size(); i++) {
            cur = go(cur, str[i]);

            // if (nodes[cur]->is_leaf) {
            //     if (i - start_positions[j] - substrings[j].size() + 1 < str.size())
            //         substr_occurs[i - start_positions[j] - substrings[j].size() + 1]++;
            // }

            for (const auto j : nodes[cur]->str_indexes) {
                // std::cout << j << ' ';
                if (nodes[cur]->is_leaf) {
                    if (i - start_positions[j] - substrings[j].size() + 1 < str.size())
                        substr_occurs[i - start_positions[j] - substrings[j].size() + 1]++;
                }
            }

            int tmp = link(cur);
            int cnt = 0;
            while (tmp != 0) {
                // fprintf(stderr, "%d\n", tmp);
                for (const auto j : nodes[tmp]->str_indexes) {
                    // std::cout << j << ' ';
                    // if (nodes[cur]->is_leaf) {
                        if (i - start_positions[j] - substrings[j].size() + 1 < str.size())
                            substr_occurs[i - start_positions[j] - substrings[j].size() + 1]++;
                    // }
                }

                tmp = link(tmp);
            }
            // std::cout << '\n';
        }
    }

    void splitToSubstrings(std::string_view mask_str, char mask_symb, std::vector<size_t>& start_positions) {
        std::string add_str;
        bool is_start = true;

        size_t str_cnt = 0;

        for (size_t i = 0; i < mask_str.size(); i++) {
            if (mask_str[i] == mask_symb) {
                if (add_str.size() != 0) {
                    addStr(str_cnt++, add_str);
                    substrings.push_back(add_str);
                }

                add_str.clear();
                is_start = true;
            } else {
                if (is_start) {
                    start_positions.push_back(i);
                    is_start = false;
                }
                add_str += mask_str[i];
            }
        }

        if (add_str.size() != 0) {
            addStr(str_cnt++, add_str);
            substrings.push_back(add_str);
        }
    }

    int go(int vert, char edge) {
        if (vert == -1 || nodes[vert]->is_leaf) return -1;
        
        if (nodes[vert]->transitions[edge] == -1) {
            if (nodes[vert]->children[edge] != -1) {
                nodes[vert]->transitions[edge] = nodes[vert]->children[edge];
            } else {
                if (vert == 0) {
                    nodes[vert]->transitions[edge] = 0;
                } else {
                    nodes[vert]->transitions[edge] = go(link(vert), edge);
                }
            }
        }

        return nodes[vert]->transitions[edge];
    }

    int link(int vert) {
        if (nodes[vert]->suf_link == -1) {
            if (vert == 0 || nodes[vert]->parent == 0) {
                nodes[vert]->suf_link = 0;
            } else {
                nodes[vert]->suf_link = go(link(nodes[vert]->parent), nodes[vert]->edge);
            }
        }

        return nodes[vert]->suf_link;
    }

    void addStr(const size_t index, const std::string& str) {
        int cur = 0;
        for (const auto symbol : str) {
            if (nodes[cur]->children[symbol] == -1) {
                nodes[cur]->children[symbol] = nodes.size();
                nodes.push_back(new AhoNode(cur, symbol));
            }

            cur = nodes[cur]->children[symbol];
        }

        nodes[cur]->is_leaf = true;
        nodes[cur]->str_indexes.push_back(index);
    }

    bool runDfs(int vert) {
        nodes[vert]->color = 1;

        for (const auto symbol : {'0', '1'}) {
            int child = go(vert, symbol);

            if (child == -1) continue;

            if (nodes[child]->color == 0) {
                if (runDfs(child)) return true;
            } else if (nodes[child]->color == 1) {
                if (nodes[child]->is_leaf) {
                    continue;
                }
                return true;
            }
        }

        nodes[vert]->color = 2;
        return false;
    } 

private:
    std::vector<AhoNode*>    nodes;
    std::vector<std::string> substrings;
};

int main() {
    Aho_Corasick aho;
    size_t N = 0;

    std::cin >> N;

    for (size_t i = 0; i < N; i++) {
        std::string str_i;
        std::cin >> str_i;
        aho.addStr(i, str_i);
    }

    if (aho.runDfs(0)) {
        std::cout << "TAK\n";
    } else {
        std::cout << "NIE\n";
    }

    return 0;
}
