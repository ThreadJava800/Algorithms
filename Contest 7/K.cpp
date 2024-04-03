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

    AhoNode()
      : children (),
        edge     ('\0'),
        batya    (-1),
        suf_link (-1),
        term_link(-1),
        word_id  (-1),
        is_leaf  (false) {
    }

    AhoNode(const int _suf_link, const int _parent, const char _edge)
      : children (),
        edge     (_edge),
        batya    (_parent),
        suf_link (_suf_link),
        term_link(-1),
        word_id  (-1),
        is_leaf  (false) {
    }

public:
    std::map<char, int> children;
    char                edge;

    int                 batya;
    int                 suf_link;
    int                 term_link;
    int                 word_id;
    bool                is_leaf;
};

class Aho_Corasick {
public:

    Aho_Corasick()
      : root (0) {
        trie.push_back(new AhoNode);
    }

    void findMaskOccur(std::string_view mask_str, char mask_symb, std::string_view substr, std::vector<int>& result) {
        std::vector<size_t> start_positions;
        std::vector<size_t> substr_occurs(substr.size(), 0);

        std::cout << "BEFORE ADD\n";
        splitToSubstrings(mask_str, mask_symb, start_positions);
        std::cout << "BEFORE INIT\n";
        init();
        std::cout << "AFTER INIT\n";

        // std::cout << substrings.size() << '\n';
        // int cnt = 0;
        // for (auto val : substrings) {
        //     std::cout << val << start_positions[cnt++] << '\n';
        // }

        findOccur(substr, start_positions, substr_occurs);

        for (size_t i = 0; i < substr_occurs.size(); i++) {
            std::cout << i << ' ' << substr_occurs[i] << '\n';
            // std::cout << substrings.size() 
            if (substr_occurs[i] == substrings.size()) {
                result.push_back(i);
            }
        }
    }

private:

    void findOccur(const std::string_view str, std::vector<size_t>& start_positions, std::vector<size_t>& substr_occurs) {
        
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

    void go(int vert) {
        if (vert == root) {
            trie[vert]->suf_link  = root;
            trie[vert]->term_link = root;
        } else if (trie[vert]->batya == root) {
            trie[vert]->suf_link = root;

            if (trie[vert]->is_leaf)
                trie[vert]->term_link = vert;
            else
                trie[vert]->term_link = trie[trie[vert]->suf_link]->term_link;
        } else {
            int  new_vert = trie[trie[vert]->batya]->suf_link;
            char new_edge = trie[vert]->edge;

            while (1) {
                if (trie[new_vert]->children.count(new_edge) > 0) {
                    trie[vert]->suf_link = trie[new_vert]->children[new_edge];
                    break;
                }

                if (new_vert == root) {
                    trie[vert]->suf_link = root;
                    break;
                }

                new_vert = trie[new_vert]->suf_link;
            }

            if (trie[vert]->is_leaf)
                trie[vert]->term_link = vert;
            else 
                trie[vert]->term_link = trie[trie[vert]->suf_link]->term_link;
        }
    }

    void init() {
        std::queue<int> bfs_queue;
        bfs_queue.push(root);

        while (!bfs_queue.empty()) {
            int cur_vert = bfs_queue.front(); bfs_queue.pop();
            go(cur_vert);

            for (const auto& [key, val] : trie[cur_vert]->children) {
                bfs_queue.push(val);
            }
        }
        
    }

    void addStr(const size_t index, const std::string& str) {
        int cur = root;

        for (const auto symbol : str) {
            if (trie[cur]->children.count(symbol) == 0) {
                trie.push_back(new AhoNode(-1, cur, symbol));
                trie[trie.size() - 1]->children[symbol] = trie.size() - 1;
            }

            cur = trie[cur]->children[symbol];
        }

        trie[cur]->is_leaf = true;
        trie[cur]->word_id = index;
        substrings.push_back(str);
    }

private:
    std::vector<std::string> substrings;

    int root;
    // int sz;
    std::vector<AhoNode*> trie;
    std::vector<int>      word_lens;
};

int main() {
    std::string substr, mask_str;
    std::cin >> substr >> mask_str;

    Aho_Corasick aho;
    std::vector<int> occurences;
    aho.findMaskOccur(substr,  '?', mask_str, occurences);

    for (auto val : occurences) {
        std::cout << val << ' ';
    }
    std::cout << '\n';

    return 0;
}