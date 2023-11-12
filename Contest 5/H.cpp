#include <queue>
#include <vector>
#include <iostream>
#include <algorithm>

using intDim2 = std::vector<std::vector<std::pair<int, int>>>;
using pairVec = std::vector<std::pair<int, int>>;

struct Trio {
    int x;
    int y;
    int ord;

    Trio(int _x, int _y, int _ord) : x(_x), y(_y), ord(_ord)    {}

    friend bool operator<(const Trio& a, const Trio& b) {
        return (a.x < b.x) || ((a.x == b.x) && (a.y < b.y)) || ((a.x == b.x) && (a.y == b.y) && (a.ord < b.ord));
    }
};

//--------------------------------------------------------------------

intDim2*          readArguments(pairVec& startEdges);
void              dfs(intDim2& paths, 
                      pairVec& times, 
                      int bfsCnt, 
                      std::pair<int, int> curPoint, 
                      std::vector<bool>& used,
                      std::vector<int>& edge);
                            
pairVec*          dfsDriver(intDim2& paths, int edgeCount);

//--------------------------------------------------------------------
int N = 0, M = 0;
intDim2* readArguments(pairVec& startEdges) {
    std::cin >> N >> M;

    intDim2* graph = new std::vector(N + 1, std::vector<std::pair<int, int>>());

    std::vector<Trio> temp;

    for (int i = 1; i <= M; i++) {
        int p1 = 0, p2 = 0;
        std::cin >> p1 >> p2;

        // p1--; p2--;

        startEdges.push_back({p1, p2});
        temp.emplace_back(Trio(std::min(p1, p2), std::max(p1, p2), i));

        // (*graph)[p1].push_back({p2, i + 1});
        // (*graph)[p2].push_back({p1, i + 1});
    }

    std::sort(temp.begin(), temp.end());

    for (int i = 0; i < M;) {
        int j = i + 1;

        while (j < M && temp[i].x == temp[j].x && temp[i].y == temp[j].y) j++;
        if (i + 1 == j) {
            (*graph)[temp[i].x].emplace_back(std::pair<int, int>(temp[i].y, temp[i].ord));
            (*graph)[temp[i].y].emplace_back(std::pair<int, int>(temp[i].x, temp[i].ord));
        }
        
        i = j;
    }

    return graph;
}

//--------------------------------------------------------------------

void dfs(intDim2& paths, 
        pairVec& times, 
        int bfsCnt, 
        std::pair<int, int> curPoint, 
        std::vector<bool>& used, 
        std::vector<int>& edge) 
{
    used[curPoint.first] = true;
    times[curPoint.first].first = times[curPoint.first].second = bfsCnt++;

    for (auto to : paths[curPoint.first]) {
        int edgeEnd = to.first;

        if (edgeEnd == curPoint.second) continue;  // back edge

        if (used[edgeEnd]) times[curPoint.first].second 
                                = std::min(times[curPoint.first].second, times[edgeEnd].first);
        else {
            dfs(paths, times, bfsCnt, {edgeEnd, curPoint.first}, used, edge);
            times[curPoint.first].second 
                                = std::min(times[curPoint.first].second, times[edgeEnd].second);

            if (times[edgeEnd].second > times[curPoint.first].first) {
                edge.push_back(to.second);
            }
        }
    }
}

std::vector<int>* dfsDriver(intDim2& paths) {
    size_t vertexCnt = paths.size();

    std::vector<bool> used  = std::vector(vertexCnt + 1, false);
    pairVec           times = std::vector(vertexCnt + 1, std::pair<int, int>(0, 0));
    std::vector<int>* edges = new std::vector<int>();

    for (size_t i = 1; i < vertexCnt; i++) {
        if (!used[i]) dfs(paths, times, 0, {i, 0}, used, *edges);
    }

    return edges;
}

enum State {
    WHITE,
    GRAY,
    BLACK
};

int componentCnt = 0;
std::vector<int> bridgeOnly;
std::vector<State> colors;
intDim2*          paths;
std::vector<int>* bridgeEdges;

void checkIfVertInSameBlock(int vert) {
    std::queue<int> checkVals;

    checkVals.push(vert);

    while (!checkVals.empty())
    {
        int checkVert = checkVals.front();
        checkVals.pop();

        colors[checkVert] = GRAY;

        for (auto to : (*paths)[checkVert]) {
            if (std::find(bridgeEdges->begin(), bridgeEdges->end(), to.second) != bridgeEdges->end()) {
                bridgeOnly[componentCnt]++;
            } else if (colors[to.first] != GRAY) {
                checkVals.push(to.first);
            }
        }
    }
}

int main() {
    pairVec           startEdges;
                      paths       = readArguments(startEdges);
                      bridgeEdges = dfsDriver(*paths);

    size_t pathCnt = startEdges.size();
    size_t edgeCnt = bridgeEdges->size();

    colors = std::vector<State>(N + 1, WHITE);
    bridgeOnly = std::vector<int>(N + 1, 0);

    std::sort(bridgeEdges->begin(), bridgeEdges->end());
    // bridgeEdges - номера мостов в списке ребер

    for (int i = 1; i <= N; i++) {
        // std::fill(colors.begin(), colors.end(), WHITE);
        if (colors[i] == WHITE) {
            componentCnt++;
            checkIfVertInSameBlock(i);
        } 
    }

    int countLeaves = 0;
    for (int i = 1; i <= N; i++) {
        if (bridgeOnly[i] == 1) countLeaves++;
    }

    std::cout << (countLeaves + 1) / 2 << '\n';

    delete paths; delete bridgeEdges;

    return 0;
}