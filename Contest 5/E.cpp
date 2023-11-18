#include <vector>
#include <iostream>
#include <algorithm>

using intDim2 = std::vector<std::vector<std::pair<int, int>>>;
using pairVec = std::vector<std::pair<int, int>>;

struct Edge {
    int from   = 0;
    int to     = 0;
    int number = 0;
};

struct GraphTime {
    int in  = 0;
    int out = 0;
};

struct GraphVertex {
    int vert   = 0;
    int parent = 0;
};

class Graph {
private:
    int timeCnt;

    std::vector<std::vector<Edge>> nonOrientedEdges;
    std::vector<GraphTime>         time;
    std::vector<bool>              used;

    void dfs(const GraphVertex curPoint, std::vector<int>& bridges) {
        used[curPoint.vert] = true;
        time[curPoint.vert].in = time[curPoint.vert].out = timeCnt++;

        size_t pathCnt = nonOrientedEdges[curPoint.vert].size();
        for (size_t i = 0; i < pathCnt; i++) {
            int edgeEnd = nonOrientedEdges[curPoint.vert][i].to;

            if (edgeEnd == curPoint.parent) continue;  // back edge

            if (used[edgeEnd]) time[curPoint.vert].out 
                                    = std::min(time[curPoint.vert].out, time[edgeEnd].in);
            else {
                dfs({edgeEnd, curPoint.vert}, bridges);
                time[curPoint.vert].out 
                                    = std::min(time[curPoint.vert].out, time[edgeEnd].out);

                if (time[edgeEnd].out > time[curPoint.vert].in) {
                    bridges.push_back(nonOrientedEdges[curPoint.vert][i].number);
                }
            }
        }
    }

public:
    explicit Graph(int N) : timeCnt(0) {
        nonOrientedEdges.resize(N);
    }

    void addEdge(int v1, int v2, int number) {
        nonOrientedEdges[v1].push_back({v1, v2, number});
        nonOrientedEdges[v2].push_back({v2, v1, number});
    }

    std::vector<int> findBridges() {
        std::vector<int> bridges;

        size_t vertexCnt = nonOrientedEdges.size();

        used.resize(vertexCnt, false);
        time.resize(vertexCnt);

        for (int i = 0; i < vertexCnt; i++) {
            if (!used[i]) dfs({i, -1}, bridges);
        }

        return bridges;
    }
};

//--------------------------------------------------------------------

Graph readArguments     ();
void  printBridgesSorted(std::vector<int>& bridges);
                        
//--------------------------------------------------------------------

Graph readArguments() {
    int N = 0, M = 0;
    std::cin >> N >> M;

    Graph graph(N);

    for (int i = 0; i < M; i++) {
        int p1 = 0, p2 = 0;
        std::cin >> p1 >> p2;

        p1--; p2--;

        graph.addEdge(p1, p2, i + 1);
    }

    return graph;
}

void printBridgesSorted(std::vector<int>& bridges) {
    size_t edgeCnt = bridges.size();

    std::cout << edgeCnt << '\n';

    std::sort(bridges.begin(), bridges.end());
    for (int i = 0; i < edgeCnt; i++) std::cout << bridges[i] << '\n';
}

//--------------------------------------------------------------------

int main() {
    Graph graph = readArguments();

    std::vector<int> bridges = graph.findBridges();
    printBridgesSorted(bridges);

    return 0;
}