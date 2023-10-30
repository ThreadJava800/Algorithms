#include <set>
#include <vector>
#include <iostream>
#include <algorithm>

using intDim2 = std::vector<std::vector<std::pair<int, int>>>;
using pairVec = std::vector<std::pair<int, int>>;

//--------------------------------------------------------------------

intDim2*          readArguments(pairVec& startEdges);
void              dfs(intDim2& paths, 
                      pairVec& times, 
                      int bfsCnt, 
                      std::pair<int, int> curPoint, 
                      std::vector<bool>& used,
                      std::set<int>& edge);
                            
std::set<int>*    dfsDriver(intDim2& paths, int edgeCount);

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

void dfs(intDim2& paths, 
        pairVec& times, 
        int bfsCnt, 
        std::pair<int, int> curPoint, 
        std::vector<bool>& used, 
        std::set<int>& edge) 
{
    used[curPoint.first] = true;
    times[curPoint.first].first = times[curPoint.first].second = bfsCnt++;

    size_t pathCnt = paths[curPoint.first].size();
    int children = 0;
    for (size_t i = 0; i < pathCnt; i++) {
        int edgeEnd = paths[curPoint.first][i].first;

        if (edgeEnd == curPoint.second) continue;  // back edge

        if (used[edgeEnd]) times[curPoint.first].second 
                                = std::min(times[curPoint.first].second, times[edgeEnd].first);
        else {
            dfs(paths, times, bfsCnt, {edgeEnd, curPoint.first}, used, edge);
            times[curPoint.first].second 
                                = std::min(times[curPoint.first].second, times[edgeEnd].second);

            if (times[edgeEnd].second >= times[curPoint.first].first && curPoint.second != -1) {
                edge.insert(curPoint.first + 1);
            }

            children++;
        }
    }

    if (curPoint.second == -1 && children > 1) edge.insert(curPoint.first + 1);
}

std::set<int>* dfsDriver(intDim2& paths) {
    size_t vertexCnt = paths.size();

    std::vector<bool> used  = std::vector(vertexCnt, false);
    pairVec           times = std::vector(vertexCnt, std::pair<int, int>(0, 0));
    std::set<int>* edges = new std::set<int>();

    for (size_t i = 0; i < vertexCnt; i++) {
        if (!used[i]) dfs(paths, times, 0, {i, -1}, used, *edges);
    }

    return edges;
}

//--------------------------------------------------------------------

int main() {
    pairVec           startEdges;
    intDim2*          paths       = readArguments(startEdges);
    std::set<int>* bridgeEdges    = dfsDriver(*paths);

    size_t pathCnt = startEdges.size();
    size_t edgeCnt = bridgeEdges->size();

    std::cout << edgeCnt << '\n';
    for (auto ver : *bridgeEdges) std::cout << ver << '\n';

    delete paths; delete bridgeEdges;

    return 0;
}