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
    AhoNode* sons       [alphabet_len];
    AhoNode* transitions[alphabet_len];

    AhoNode* parent         = nullptr;
    AhoNode* suf_link       = nullptr;
    AhoNode* short_suf_link = nullptr;

    char edge    = '\0';
    bool is_leaf = false;

    std::vector<int> leaves;

    AhoNode() {
        std::memset(sons, 0, sizeof(sons));
        std::memset(sons, 0, sizeof(transitions));
    }
};

class Aho_Corasick {
public:
    Aho_Corasick()
      : root (new AhoNode) {}

    void addString(const std::string_view str, const int pattern_num) {
        AhoNode* cur = root;

        for (size_t i = 0; i < str.size(); i++) {
            const char edge = str[i];
            if (!cur->sons[edge]) {
                cur->sons[edge] = new AhoNode;

                cur->sons[edge]->suf_link       = nullptr;
                cur->sons[edge]->short_suf_link = nullptr;
                cur->sons[edge]->parent         = cur;
                cur->sons[edge]->edge           = edge;
                cur->sons[edge]->is_leaf        = false;
            }

            cur = cur->sons[edge];
        }

        cur->is_leaf = true;
        cur->leaves.push_back(pattern_num);
    }

    AhoNode* getSufLink(AhoNode* vert) {
        if (!vert->suf_link) {
            if (vert->parent == root || vert == root)
                vert->suf_link = root;
            else
                vert->suf_link = getLink(getSufLink(vert->parent), vert->edge);
        }

        return vert->suf_link;
    }

    AhoNode* getShortLink(AhoNode* vert) {
        if (!vert->short_suf_link) {
            AhoNode* suf_link = getSufLink(vert);

            if (suf_link->is_leaf)
                vert->short_suf_link = suf_link;
            else if (suf_link == root)
                vert->short_suf_link = root;
            else
                vert->short_suf_link = getShortLink(suf_link);
        }

        return vert->short_suf_link;
    }

    AhoNode* getLink(AhoNode* vert, const char edge) {
        if (!vert->transitions[edge]) {
            if (vert->sons[edge])
                vert->transitions[edge] = vert->sons[edge];
            else if (vert == root)
                vert->transitions[edge] = root;
            else
                vert->transitions[edge] = getLink(getSufLink(vert), edge);
        }

        return vert->transitions[edge];
    }

    void findOccur(const std::string_view str, std::vector<int>& occurs) {
        AhoNode* cur = root;

        for (size_t i = 0; i < str.size(); i++) {
            const char edge = str[i];
            cur = getLink(cur, edge);

            if (cur->is_leaf)
                occurs.push_back(i);

            // while (cur != root) {
            //     occurs.push_back(i);
            //     cur = getShortLink(cur);
            // }
            
        }
    }

private:
    AhoNode* root;
};

int main() {
    // std::string substr, mask_str;
    // std::cin >> substr >> mask_str;

    Aho_Corasick aho;
    std::vector<int> occurences;
    aho.addString("he", 0);
    aho.addString("she", 1);
    aho.addString("hers", 2);
    aho.addString("his", 3);
    // aho.addString("he");
    // aho.findMaskOccur(substr,  '?', mask_str, occurences);

    aho.findOccur("ahishers", occurences);
    for (auto val : occurences) {
        std::cout << val << ' ';
    }
    std::cout << '\n';

    return 0;
}