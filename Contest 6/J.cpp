#include <bits/stdc++.h>

class Graph {
private:
    std::vector<std::vector<long>> data;
    std::vector<bool>              used;
    std::vector<long>              matching;

    long m, n;

public:
    explicit Graph(long _n, long _m) : m(_m), n(_n) {
        data    .resize(n);
        used    .resize(n, false);
        matching.resize(m, -1);
    }

    void pushEdge(long _from, long _to) {
        data[_from - 1].push_back(_to - 1);
    }

    bool _dfsDoNotUse(int check_vert) {
        if (used[check_vert]) return false;

        used[check_vert] = true;

        for (auto vert_to : data[check_vert]) {
            if (matching[vert_to] == -1 || _dfsDoNotUse(matching[vert_to])) {
                matching[vert_to] = check_vert;
                return true;
            }
        }

        return false;
    }

    void printMax() {
        int cnt = 0;
        for (long i = 0; i < n; i++) {
            used.assign(n, false);
            if(_dfsDoNotUse(i)) cnt++;
        }

        std::cout << cnt << '\n';
        for (long i = 0; i < m; i++) {
            if (matching[i] != -1) std::cout << matching[i] + 1 << ' ' << i + 1 << '\n';
        }
    }
};

int main() {
    int n = 0, m = 0;
    std::cin >> n >> m;

    Graph graph = Graph(n, m);
    for (int i = 1; i <= n; i++) {
        int val = -1;
        while (val) {
            std::cin >> val;
            if (val != 0) graph.pushEdge(i, val);
        }
    }

    graph.printMax();

    return 0;
}