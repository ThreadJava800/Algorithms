#include <iostream>
#include <string>
#include <string_view>
#include <vector>

class Node;

const size_t alphabet_len = 256;
Node* root = nullptr;

class Node {
public:
    explicit Node()
        : parent             (nullptr),
          suffix_link        (nullptr),
          compressed_suf_link(nullptr),
          edge_to_parent     (0),
          is_leaf            (false) {
        
    }

    void addString(std::string_view str) {
        Node* cur = root;
        for (size_t i = 0; i < str.length(); i++) {
            char edge = str[i];
            if (!cur->sons[edge]) {
                cur->sons[edge] = new Node;
                cur->sons[edge]->suffix_link         = nullptr;
                cur->sons[edge]->compressed_suf_link = nullptr;
                cur->sons[edge]->parent              = this;
                cur->sons[edge]->edge_to_parent      = edge;
                cur->sons[edge]->is_leaf             = false;
            }
            cur = cur->sons[edge];
        }

        cur->is_leaf = true;
    }

private:

    Node* getSuffixLink() {
        if (!suffix_link) {
            if (this == root || parent == root) 
                suffix_link = root;
            else
                suffix_link = parent->getSuffixLink()->getLink(edge_to_parent);
        }

        return suffix_link;
    }

    Node* getLink(char edge) {
        if (!transitions[edge]) {
            if (sons[edge])
                transitions[edge] = sons[edge];
            else if (this == root)
                transitions[edge] = root;
            else
                transitions[edge] = getSuffixLink()->getLink(edge);
        }

        return transitions[edge];
    }

    Node* getCompressed() {
        if (!compressed_suf_link) {
            Node* suf_link = getSuffixLink();
            if (suf_link->is_leaf)
                compressed_suf_link = suf_link;
            else if (suf_link == root)
                compressed_suf_link = root;
            else 
                compressed_suf_link = suf_link->getCompressed();
        }

        return compressed_suf_link;
    }

private:
    Node* sons       [alphabet_len] = {nullptr};
    Node* transitions[alphabet_len] = {nullptr};
    Node* parent;
    Node* suffix_link;
    Node* compressed_suf_link;

    char edge_to_parent;
    bool is_leaf;

    std::vector<int> leaf_pattern_numbers;

};

int main() {
    std::vector<std::string> viruses;
    size_t N = 0;
    std::cin >> N;

    root = new Node;

    for (size_t i = 0; i < N; i++) {
        std::string str_i;
        std::cin >> str_i;

        viruses.emplace_back(str_i);
        root->addString(str_i);
    }

    // Node::root->addString();

    return 0;
}