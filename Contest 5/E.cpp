#include <vector>
#include <iostream>
#include <algorithm>

using intDim2 = std::vector<std::vector<std::pair<int, int>>>;
using pairVec = std::vector<std::pair<int, int>>;

//--------------------------------------------------------------------

intDim2*          readArguments(pairVec& startEdges);
void              dfs(const intDim2& paths, 
                            pairVec& times, 
                            int bfsCnt, 
                      const std::pair<int, int> curPoint, 
                            std::vector<bool>& used,
                            std::vector<int>& edge);
                            
std::vector<int>* dfsDriver(const intDim2& paths);

//--------------------------------------------------------------------

intDim2* readArguments(pairVec& startEdges) {
    int N = 0, M = 0;
    std::cin >> N >> M;

    intDim2* graph = new std::vector(N, std::vector<std::pair<int, int>>());

    for (int i = 0; i < M; i++) {
        int p1 = 0, p2 = 0;
        std::cin >> p1 >> p2;

        p1--; p2--;

        startEdges.push_back({p1, p2});

        (*graph)[p1].push_back({p2, i + 1});
        (*graph)[p2].push_back({p1, i + 1});
    }

    return graph;
}

//--------------------------------------------------------------------

void dfs(const intDim2& paths, 
               pairVec& times, 
               int bfsCnt, 
         const std::pair<int, int> curPoint, 
               std::vector<bool>& used, 
               std::vector<int>& edge) 
{
    used[curPoint.first] = true;
    times[curPoint.first].first = times[curPoint.first].second = bfsCnt++;

    size_t pathCnt = paths[curPoint.first].size();
    for (size_t i = 0; i < pathCnt; i++) {
        int edgeEnd = paths[curPoint.first][i].first;

        if (edgeEnd == curPoint.second) continue;  // back edge

        if (used[edgeEnd]) times[curPoint.first].second 
                                = std::min(times[curPoint.first].second, times[edgeEnd].first);
        else {
            dfs(paths, times, bfsCnt, {edgeEnd, curPoint.first}, used, edge);
            times[curPoint.first].second 
                                = std::min(times[curPoint.first].second, times[edgeEnd].second);

            if (times[edgeEnd].second > times[curPoint.first].first) {
                edge.push_back(paths[curPoint.first][i].second);
            }
        }
    }
}

std::vector<int>* dfsDriver(const intDim2& paths) {
    size_t vertexCnt = paths.size();

    std::vector<bool> used  = std::vector(vertexCnt, false);
    pairVec           times = std::vector(vertexCnt, std::pair<int, int>(0, 0));
    std::vector<int>* edges = new std::vector<int>();

    for (size_t i = 0; i < vertexCnt; i++) {
        if (!used[i]) dfs(paths, times, 0, {i, -1}, used, *edges);
    }

    return edges;
}

//--------------------------------------------------------------------

int main() {
    pairVec           startEdges;
    intDim2*          paths       = readArguments(startEdges);
    std::vector<int>* bridgeEdges = dfsDriver(*paths);

    size_t pathCnt = startEdges.size();
    size_t edgeCnt = bridgeEdges->size();

    std::cout << edgeCnt << '\n';

    std::sort(bridgeEdges->begin(), bridgeEdges->end());
    for (int i = 0; i < edgeCnt; i++) std::cout << (*bridgeEdges)[i] << '\n';

    delete paths; delete bridgeEdges;

    return 0;
}