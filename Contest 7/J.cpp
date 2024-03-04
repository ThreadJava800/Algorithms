#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>

const size_t alphabet_len = 256;
const size_t max_node_cnt = 256;

struct AhoNode {
    AhoNode(int _par = -1, char _edge = 0) 
        : parent(_par),
          edge  (_edge) {
        std::fill(std::begin(sons), std::end(sons), -1);
        std::fill(std::begin(transmissions), std::end(transmissions), -1);
    }


    int sons         [alphabet_len];
    int transmissions[alphabet_len];
    
    int  parent;
    bool is_leaf;
    int  suffix_link;
    char edge;

    int color = 0; // white
};

std::vector<AhoNode> nodes;

class AhoCorasick {
public:
    // void AhoCorasick(int par = -1, char edge = 10) {
    //     nodes[0].parent      = -1;
    //     nodes[0].suffix_link = -1;
    //     size                 =  1;

    //     std::fill(std::begin(nodes[0].sons), std::end(nodes[0].sons), -1);
    //     std::fill(std::begin(nodes[0].sons), std::end(nodes[0].sons), -1);
    //     // memset(nodes[0].sons         , 255, sizeof(nodes[0].sons));
    //     // memset(nodes[0].transmissions, 255, sizeof(nodes[0].transmissions));
    // }

    void addString(const std::string_view str) {
        int cur_node = 0;

        for (size_t i = 0; i < str.length(); i++) {
            char edge = str[i];
            // std::cerr << nodes[cur_node].sons[edge] << '\n';
            if (nodes[cur_node].sons[edge] == -1) {
                nodes[cur_node].sons[edge] = nodes.size();
                nodes.push_back(AhoNode(cur_node, edge));
            }
            // std::cerr << nodes[cur_node].sons[edge] << '\n';
            cur_node = nodes[cur_node].sons[edge];
        }

        nodes[cur_node].is_leaf = true;
    }

public:
    int transmit(int node_ind, char edge) {
        if (nodes[node_ind].transmissions[edge] == -1) {
            if (nodes[node_ind].sons[edge] != -1)
                nodes[node_ind].transmissions[edge] = nodes[node_ind].sons[edge];
            else {
                if (node_ind == 0)
                    nodes[node_ind].transmissions[edge] = 0; 
                else 
                    nodes[node_ind].transmissions[edge] = transmit(getLink(node_ind), edge);
            }
        }

        return nodes[node_ind].transmissions[edge];
    }

    int getLink(int node_ind) {
        if (nodes[node_ind].suffix_link == -1) {
            if (node_ind == 0 || nodes[node_ind].parent == 0)
                nodes[node_ind].suffix_link = 0;
            else
                nodes[node_ind].suffix_link = transmit(getLink(nodes[node_ind].parent), nodes[node_ind].edge);
        } 

        return nodes[node_ind].suffix_link;
    }
};

bool doDFS(AhoCorasick& aho, int cur_node) {
    nodes[cur_node].color = 1; // grey
    if (nodes[cur_node].is_leaf) return false;

    for (int i = 0; i < alphabet_len; i++) {
        // if (nodes[cur_node].suffix_link == -1) continue;
        int to = aho.transmit(cur_node, i);

        if (nodes[to].color == 0) {
            if (doDFS(aho, to)) return true;
        } else if (nodes[to].color == 1) {
            if (nodes[to].is_leaf) return false;
            return true;
        }
    }
    nodes[cur_node].color = 2;

    return false;
}

bool findOccur(std::vector<std::string>& strings) {
    AhoCorasick aho;

    for (size_t i = 0; i < strings.size(); i++) {
        aho.addString(strings[i]);
    }
    return doDFS(aho, 0);
}

int main() {
    std::vector<std::string> viruses;
    size_t N = 0;
    std::cin >> N;

    AhoNode node;
    nodes.push_back(node);

    for (size_t i = 0; i < N; i++) {
        std::string str_i;
        std::cin >> str_i;

        viruses.push_back(str_i);
    }

    if (findOccur(viruses)) {
        std::cout << "TAK\n";
    } else {
        std::cout << "NIE\n";
    }

    return 0;
}