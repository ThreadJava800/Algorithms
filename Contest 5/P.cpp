#include <queue>
#include <vector>
#include <limits>
#include <iostream>

static const int INF = __INT_MAX__;

struct Edge {
    int from;
    int to;

    explicit Edge()                   : from(-1)   , to(-1) {}
    explicit Edge(int _from, int _to) : from(_from), to(_to) {}
};

class Graph {
private:
    int                                           vert_cnt;
    std::vector<int>                              vertexes;
    std::vector<std::vector<std::pair<int, int>>> edges;

public:
    explicit Graph(int _vert_cnt) : vert_cnt(_vert_cnt) {
        vertexes.resize(_vert_cnt, -1);
        edges   .resize(_vert_cnt, std::vector<std::pair<int, int>>());

        for (int i = 0; i < vert_cnt; i++) edges[i].push_back({i, 0});
    }

    void addEdge(int _from, int _to, int _weight) {
        edges[_from].push_back({_to  , _weight});
        edges[_to]  .push_back({_from, _weight});
    }

    int dijkstra(int start_vert, int end_vert) {
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> heap;
        std::vector<int> d(vert_cnt, INF);

        d[start_vert] = 0;
        heap.push({start_vert, 0});

        while (!heap.empty())
        {
            int cur_vert = heap.top().first; heap.pop();

            for (auto edge : edges[cur_vert]) {
                int to = edge.first;
                int w  = edge.second;

                if (d[cur_vert] + w < d[to]) {
                    d[to] = d[cur_vert] + w;
                    heap.push({to, d[to]});
                }
            }
        }
        
        return d[end_vert];
    }
};

int main() {
    int n = 0;
    std::cin >> n;

    Graph graph(n);

    for (int i = 0; i < n - 1; i++) {
        int v1 = 0, v2 = 0;
        std::cin >> v1 >> v2;

        graph.addEdge(--v1, --v2, 1);
    }

    std::cin >> n;
    for (int i = 0; i < n; i++) {
        int start_vert = 0, end_vert = 0;
        std::cin >> start_vert >> end_vert;

        --start_vert; --end_vert;

        std::cout << graph.dijkstra(std::min(start_vert, end_vert), std::max(start_vert, end_vert)) << '\n';
    }

    return 0;
}