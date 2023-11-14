#include <cmath>
#include <queue>
#include <vector>
#include <limits>
#include <iostream>

static const long long INF = __LONG_LONG_MAX__;

struct Edge {
    long long from;
    long long to;

    explicit Edge()                   : from(-1)   , to(-1) {}
    explicit Edge(long long _from, long long _to) : from(_from), to(_to) {}
};

long long timer = 0;

class Graph {
public:
    long long root = 0;

private:
    long long                                           vert_cnt;
    std::vector<long long>                              vertexes;
    std::vector<std::vector<std::pair<long long, long long>>> edges;
    std::vector<std::vector<long long>>                 dp;
    std::vector<long long>                              parent;
    std::vector<long long>                              depth;
    std::vector<long long>                              tin;
    std::vector<long long>                              tout;
    std::vector<long long>                              degs;
    std::vector<bool>                                   colors;

    inline bool isUpper(long long v1, long long v2) {
        return tin[v1] <= tin[v2] && tout[v1] >= tout[v2];
    }

    long long lca(long long v, long long u) {
        if (isUpper(v, u)) return v;
        if (isUpper(u, v)) return u;

        for (long long i = std::ceil(std::log2(vert_cnt)); i >= 0; i--) {
            if (!isUpper(dp[v][i], u)) v = dp[v][i];
        }

        return dp[v][0];
    }

public:
    explicit Graph(long long _vert_cnt) : vert_cnt(_vert_cnt) {
        vertexes   .resize(_vert_cnt, -1);
        edges      .resize(_vert_cnt, std::vector<std::pair<long long, long long>>());
        dp         .resize(_vert_cnt, std::vector<long long>(std::ceil(std::log2(vert_cnt)) + 1, 0));
        parent     .resize(_vert_cnt, -1);
        depth      .resize(_vert_cnt, -1);
        tin        .resize(_vert_cnt, 0);
        tout       .resize(_vert_cnt, 0);
        colors     .resize(_vert_cnt, false);
        degs       .resize(_vert_cnt, 0);     

        root = 0;

        // for (long long i = 0; i < vert_cnt; i++) edges[i].push_back({i, 0});
    }

    void updateParents(long long vert, std::vector<bool>& visited) {
        if (vert == -1) return;

        for (auto edge : edges[vert]) {
            if (visited[edge.first]) continue;

            visited[edge.first] = true;
            parent [edge.first] = vert;
            updateParents(edge.first, visited);
        }
    }

    void updateDepth(long long vert, std::vector<bool>& visited) {
        if (parent[vert] != -1) depth[vert] = depth[parent[vert]] + 1;
        visited[vert] = true;

        for (auto edge : edges[vert]) {
            if (!visited[edge.first]) updateDepth(edge.first, visited);

        }
    }

    void addEdge(long long _from, long long _to, long long _weight) {
        edges[_from].push_back({_to  , _weight});
        edges[_to]  .push_back({_from, _weight});
    }

    void countParDepth() {
        std::vector<bool> visited1(vert_cnt, false);
        updateParents(root, visited1);

        // if (parent[_to] != -1 && depth[parent[_to]] == 0) root = parent[_to];

        std::vector<bool> visited2(vert_cnt, false);
        updateDepth(root, visited2);
    }

    void prepare(long long vert, long long par = 0) {
        tin[vert] = ++timer;

        colors[vert] = true;

        for (long long i = 0; i < std::ceil(std::log2(vert_cnt)) + 1; i++) {
            // if (i > 0) std::cerr << vert << ' ' << i << ' ' << dp.size() << ' ' << dp[vert].size() << ' ' << dp[v][i - 1] << '\n';
            if (i == 0) dp[vert][i] = par;
            else        dp[vert][i] = dp[dp[vert][i - 1]][i - 1];
        }

        for (auto edge : edges[vert]) {
            long long to = edge.first;

            if (to != par && !colors[to]) prepare(to, vert);
        }

        tout[vert] = ++timer;
    }

    long long dijkstra(long long start_vert, long long end_vert) {
        long long common_batya = lca(start_vert, end_vert);
        // std::cout << common_batya << ' ' << depth[common_batya] << ' ' << depth[start_vert] << ' ' << depth[end_vert] << '\n';

        return std::abs(depth[start_vert] - depth[common_batya]) + std::abs(depth[end_vert] - depth[common_batya]);
    }

    void printTest() {
        for (int i = 0; i < vert_cnt; i++) std::cout << i << ' ' << parent[i] << '\n';
    }
};

int main() {
    long long n = 0;
    std::cin >> n;

    Graph graph(n);

    for (long long i = 0; i < n - 1; i++) {
        long long v1 = 0, v2 = 0;
        std::cin >> v1 >> v2;

        --v1; --v2;

        graph.addEdge(std::min(v1, v2), std::max(v1, v2), 1);
    }

    graph.countParDepth();
    graph.prepare(graph.root);
    // graph.printTest();

    std::cin >> n;
    for (long long i = 0; i < n; i++) {
        long long start_vert = 0, end_vert = 0;
        std::cin >> start_vert >> end_vert;

        --start_vert; --end_vert;

        std::cout << graph.dijkstra(std::min(start_vert, end_vert), std::max(start_vert, end_vert)) << '\n';
    }

    return 0;
}