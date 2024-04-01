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
      :  last_state (0),
         size       (0) {
        states.resize(str.size() * 2 + 1, SuffixNode(0, -1));
        states[0].len = 0; states[0].link = -1;
        size++;

        for (auto c : str)
            addSymbol(c);
    }

    size_t getSubstrPaths() {
        size_t result = 0;
        for (int i = 1; i < size; i++) {
            result += states[i].len - states[states[i].link].len;
        }

        return result;
    }

    void traverseAutomaton() {
        std::cout << "Traversing Suffix Automaton:\n";
        for (int i = 0; i < states.size(); ++i) {
            std::cout << "State " << i << ", Length: " << states[i].len << ", Suffix Link: " << states[i].link << "\n";
            for (const auto& transition : states[i].son) {
                std::cout << " Transition on '" << transition.first << "' to State " << transition.second << "\n";
            }
        }
    }

private:
    size_t getSubstrPathsDP(int current, std::vector<int>& d, std::vector<bool>& w) {
        if (w[current]) return d[current];

        size_t result = 0;
        w[current] = true;

        for (const auto& [key, value] : states[current].son) {
            result += getSubstrPathsDP(value, d, w);
        }

        d[current] = result;
        return result;
    }

    void addSymbol(char symbol) {
        int current = size++;

        states[current].len = states[last_state].len + 1;

        int prev = last_state;
        for (; prev >= 0 && states[prev].son.find(symbol) == states[prev].son.end(); prev = states[prev].link) {
            states[prev].son[symbol] = current;
        }

        if (prev == -1) states[current].link = 0;
        else  {
            int next_state = states[prev].son[symbol];

            if (states[prev].len + 1 == states[next_state].len)
                states[current].link = next_state;
            else {
                int copy = size++;

                // states[copy] = SuffixNode(states[prev].len + 1, states[next_state].link);
                states[copy].len  = states[prev].len + 1;
                states[copy].link = states[next_state].link;
                states[copy].son  = states[next_state].son;

                while (prev >= 0 && states[prev].son[symbol] == next_state) {
                    states[prev].son[symbol] = copy;
                    prev = states[prev].link;
                }
                
                states[next_state].link = copy;
                states[current]   .link = copy;
            }
        }

        last_state = current;
    }

    std::vector<SuffixNode> states;
    int                     last_state;
    int                     size;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::string str;
    std::cin >> str;
    SuffixAutomation suf_automat(str);

    // suf_automat.traverseAutomaton();
    std::cout << suf_automat.getSubstrPaths() << '\n';

    return 0;
}