#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <ranges>

static const int alphabet_len = 26;

struct SuffixNode {
    SuffixNode(int _len = 0, int _link = -1) 
      :  len  (_len),
         link (_link) {
    }

    int                 len;
    int                 link;
    std::map<char, int> son;
};

class SuffixAutomation {
public:
    SuffixAutomation(std::string_view str)
      :  last_state (0) {
        nodes.emplace_back();

        for (auto symbol : str)
            addSymbol(symbol);
    }

    size_t getSubstrPaths() {
        std::vector<int>  d(nodes.size(), 0);
        std::vector<bool> w(nodes.size(), false);
        size_t result = 0;

        d[0] = 1; w[0] = true;

        return getSubstrPathsDP(last_state, d, w);
    }

    void traverseAutomaton() {
        std::cout << "Traversing Suffix Automaton:\n";
        for (int i = 0; i < nodes.size(); ++i) {
            std::cout << "State " << i << ", Length: " << nodes[i].len << ", Suffix Link: " << nodes[i].link << "\n";
            for (const auto& transition : nodes[i].son) {
                std::cout << " Transition on '" << transition.first << "' to State " << transition.second << "\n";
            }
        }
    }

private:
    size_t getSubstrPathsDP(int current, std::vector<int>& d, std::vector<bool>& w) {
        if (w[current]) return d[current];

        size_t result = 0;
        w[current] = true;


        auto keys_it = std::views::keys(nodes[current].son);
        std::vector<char> keys(keys_it.begin(), keys_it.end());

        for (auto symbol : keys) {
            result += getSubstrPathsDP(nodes[current].son[symbol], d, w);
        }

        d[current] = result;
        return result;
    }

    void addSymbol(char symbol) {
        SuffixNode new_node(nodes[last_state].len + 1);

        int cur_state = last_state;
        while (cur_state != -1 
            && nodes[cur_state].son.find(symbol) == nodes[cur_state].son.end()) {
            
            nodes[cur_state].son[symbol] = nodes.size();
            cur_state = nodes[cur_state].link;
        }
        

        if (cur_state == -1) new_node.link = 0;
        else {
            int next_state = nodes[cur_state].son[symbol];

            if (nodes[cur_state].len + 1 == nodes[next_state].len) {
                new_node.link = next_state;
            } else {
                SuffixNode next_copy = nodes[next_state];
                next_copy.len = nodes[cur_state].len + 1;
                nodes.push_back(next_copy);

                while (cur_state != -1 && nodes[cur_state].son[symbol] == next_state) {
                    nodes[cur_state].son[symbol] = nodes.size() - 1;
                    cur_state = nodes[cur_state].link;
                }
                
                new_node.link = nodes.size() - 1;
                nodes[next_state].link = new_node.link;
            }
        }

        last_state = nodes.size();
        nodes.push_back(new_node);
    }

    std::vector<SuffixNode> nodes;
    int                     last_state;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::string str;
    std::cin >> str;
    SuffixAutomation suf_automat(str);

    suf_automat.traverseAutomaton();
    std::cout << suf_automat.getSubstrPaths() << '\n';

    return 0;
}