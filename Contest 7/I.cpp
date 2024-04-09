#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

struct Node {
    std::vector<int> children;
    std::vector<int> transitions;

    char   parent_path;
    int    parent;
    int    suffix_link;
    bool   is_leaf;

    explicit Node(int _parent = -1, int _suffix_link = -1)
        : parent_path (0), 
          parent      (_parent),
          suffix_link (_suffix_link),
          is_leaf     (false) {
        children   .resize(255, 255);
        transitions.resize(255, 255);
    }
};

const int MAX_N = 30000;

class Aho {
public:
    explicit Aho() 
      : cur_cnt(1) {
        nodes.resize(MAX_N, std::make_shared<Node>());
    }

    void addString(const std::string_view str) {
        int cur_node = 0;
        for (size_t i = 0; i < str.size(); i++) {
            char cur = str[i];
            if (nodes[cur_node]->children[cur] == -1) {
                nodes[cur_cnt] = std::make_shared<Node>(cur_node, -1);
                nodes[cur_cnt]->parent_path    = cur;
                nodes[cur_node]->children[cur] = cur_cnt++;
            }

            cur_node = nodes[cur_node]->children[cur];
        }
        nodes[cur_node]->is_leaf = true;
    }

    int getSufLink(const int cur_node) {
        if (nodes[cur_node]->suffix_link == -1) {
            if (cur_node == 0 || nodes[cur_node]->parent == 0) {
                nodes[cur_node]->suffix_link = 0;
            } else {
                nodes[cur_node]->suffix_link = go(getSufLink(nodes[cur_node]->parent), nodes[cur_node]->parent_path);
            }
        }

        return nodes[cur_node]->suffix_link;
    }

    int go(const int cur_node, const char path) {
        if (nodes[cur_node]->transitions[path] == -1) {
            if (nodes[cur_node]->children[path] != -1) {
                nodes[cur_node]->transitions[path] = nodes[cur_node]->children[path];
            } else {
                if (cur_node == 0) {
                    nodes[cur_node]->transitions[path] = 0;
                } else {
                    nodes[cur_node]->transitions[path] = go(getSufLink(cur_node), path);
                }
            }
        }

        return nodes[cur_node]->transitions[path];
    }

    int getRoot() {
        return _getRoot(0);
    }

    int _getRoot(int node) {
        while (nodes[node] && nodes[node]->parent != -1) {
            node = nodes[node]->parent;
        }
        return node;
    }

private:
    std::vector<std::shared_ptr<Node>> nodes;
    size_t cur_cnt;
};

int main() {
    

    return 0;
}