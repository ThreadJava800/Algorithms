#include <iostream>
#include <vector>
#include <assert.h>

enum DFSColors {
    BLACK,
    WHITE,
    GRAY
};

void topologicalSort(std::vector<DFSColors>& colors, std::vector<std::vector<int>>& tree, std::vector<int>& cycleP, int nodeId, bool* isCycle);

void topologicalSort(std::vector<DFSColors>& colors, std::vector<std::vector<int>>& tree, std::vector<int>& cycleP, int nodeId, bool* isCycle) {
    assert(isCycle);
    
    colors[nodeId] = GRAY;
    
    size_t curChildCnt = tree[nodeId].size();
    for (size_t i = 0; i < curChildCnt; i++) {
        int checkChild = tree[nodeId][i];

        if (colors[checkChild] == WHITE) {
            topologicalSort(colors, tree, cycleP, checkChild, isCycle);
        }
        if (colors[checkChild] == GRAY) {
            *isCycle = true;
        }
    }

    colors[nodeId] = BLACK;
    cycleP.push_back(nodeId);
}

int main() {
    int N = 0, M = 0;
    std::cin >> N >> M;

    std::vector<std::vector<int>> paths(N + 1, std::vector<int>());

    for (int i = 0; i < M; i++) {
        int p1 = 0, p2 = 0;
        std::cin >> p1 >> p2;

        paths[p1].push_back(p2);
    }

    std::vector<DFSColors> colors(N + 1, WHITE);
    std::vector<int>       cycleP;
    
    bool isCycle = false;

    for (int i = 1; i <= N; i++) {
        if (colors[i] == WHITE) topologicalSort(colors, paths, cycleP, i, &isCycle);
    }
    
    if (isCycle) std::cout << -1;
    else {
        for (int i = cycleP.size() - 1; i >= 0; i--)
            std::cout << cycleP[i] << ' ';
    }
    std::cout << '\n';

    return 0;
}