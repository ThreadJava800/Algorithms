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
        parent.resize(n, -1);
        ranks .resize(n,  1);
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
    int n = 0, m = 0;
    std::cin >> n >> m;

    MSTGraph mst(n);

    for (int i = 0; i < m; i++) {
        int vert1 = 0, vert2 = 0, weight = 0;
        std::cin >> vert1 >> vert2 >> weight;

        mst.addEdge(--vert1, --vert2, weight);
    }

    std::cout << mst.findMSTWeight() << '\n';

    return 0;
}