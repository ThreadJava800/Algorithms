#include <vector>
#include <iostream>
#include <algorithm>

struct Trio {
    int x = 0;
    int y = 0;
    int w = 0;

    explicit Trio(int _x, int _y, int _w) : x(_x), y(_y), w(_w) {}

    friend bool operator<(const Trio& a, const Trio& b) {
        return a.w < b.w;
    }
};

class DSU {
private:
    std::vector<int> parent;
    std::vector<int> ranks;     // высота дерева представителя

public:
    explicit DSU(int n) {
        parent.resize(n + 1, -1);
        ranks .resize(n + 1,  1);
    }

    int find(int val) {
        if (parent[val] == -1) return val;

        parent[val] = find(parent[val]);
        return parent[val];
    }

    void unite(int S, int T) {
        int par1 = find(S);
        int par2 = find(T);

        // в разных наборах
        if (par1 != par2) {
            if      (ranks[par1] < ranks[par2]) parent[par1] = par2;
            else if (ranks[par1] > ranks[par2]) parent[par2] = par1;
            else    {
                parent[par1] = par2;
                ranks[par2]++; 
            }
        }
    }
};

class MSTGraph {
private:
    std::vector<Trio> edges;
    size_t vertexCnt = 0;

public:
    explicit MSTGraph(int _vertexCnt) : vertexCnt(_vertexCnt) {}

    void addEdge(int _x, int _y, int _w) {
        edges.emplace_back(Trio(_x, _y, _w));
    }

    void addFictive(int _x, int _weight) {
        edges.emplace_back(Trio(vertexCnt, _x, _weight));
    }

    int findMSTWeight() {
        std::sort(edges.begin(), edges.end());

        int mstWeight = 0;

        DSU disjoinSetUnion = DSU(vertexCnt);

        for (Trio edge : edges) {
            if (disjoinSetUnion.find(edge.x) != disjoinSetUnion.find(edge.y)) {
                disjoinSetUnion.unite(edge.x, edge.y);
                mstWeight += edge.w;
            }
        }

        return mstWeight;
    }
};

int main() {
    int n = 0;
    std::cin >> n;

    MSTGraph mst(n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int weight;
            std::cin >> weight;

            mst.addEdge(i, j, weight);
        }
    }

    for (int i = 0; i < n; i++) {
        int weight;
        std::cin >> weight;

        mst.addFictive(i, weight);
    }

    std::cout << mst.findMSTWeight() << '\n';

    return 0;
}