#include <vector>
#include <iostream>
#include <algorithm>

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
public:
    explicit Graph(int N) : timeCnt(0) {
        nonOrientedEdges.resize(N);
    }

    void addEdge(int v1, int v2, int number) {
        nonOrientedEdges[v1].push_back({v1, v2, number});
        nonOrientedEdges[v2].push_back({v2, v1, number});
    }

public:
    int timeCnt;

    std::vector<std::vector<Edge>> nonOrientedEdges;
    std::vector<GraphTime>         time;
    std::vector<bool>              used;
};

//--------------------------------------------------------------------

std::vector<int> findBridges       (Graph& graph);
void             _findBridgesDFS   (Graph& graph, const GraphVertex curPoint, std::vector<int>& bridges);
Graph            readArguments     (std::istream& stream);
void             printBridgesSorted(std::vector<int>& bridges);
                        
//--------------------------------------------------------------------

std::vector<int> findBridges(Graph& graph) {
    std::vector<int> bridges;

    size_t vertexCnt = graph.nonOrientedEdges.size();

    graph.used.resize(vertexCnt, false);
    graph.time.resize(vertexCnt);

    for (int i = 0; i < vertexCnt; i++) {
        if (!graph.used[i]) _findBridgesDFS(graph, {i, -1}, bridges);
    }

    return bridges;
}

void _findBridgesDFS(Graph& graph, const GraphVertex curPoint, std::vector<int>& bridges) {
    graph.used[curPoint.vert] = true;
    graph.time[curPoint.vert].in = graph.time[curPoint.vert].out = graph.timeCnt++;

    size_t pathCnt = graph.nonOrientedEdges[curPoint.vert].size();
    for (size_t i = 0; i < pathCnt; i++) {
        int edgeEnd = graph.nonOrientedEdges[curPoint.vert][i].to;

        if (edgeEnd == curPoint.parent) continue;  // back edge

        if (graph.used[edgeEnd]) graph.time[curPoint.vert].out 
                                = std::min(graph.time[curPoint.vert].out, graph.time[edgeEnd].in);
        else {
            _findBridgesDFS(graph, {edgeEnd, curPoint.vert}, bridges);
            graph.time[curPoint.vert].out 
                                = std::min(graph.time[curPoint.vert].out, graph.time[edgeEnd].out);

            if (graph.time[edgeEnd].out > graph.time[curPoint.vert].in) {
                bridges.push_back(graph.nonOrientedEdges[curPoint.vert][i].number);
            }
        }
    }
}

Graph readArguments(std::istream& stream) {
    int N = 0, M = 0;
    stream >> N >> M;

    Graph graph(N);

    for (int i = 0; i < M; i++) {
        int p1 = 0, p2 = 0;
        stream >> p1 >> p2;

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
    Graph graph = readArguments(std::cin);

    std::vector<int> bridges = findBridges(graph);
    printBridgesSorted(bridges);

    return 0;
}