#include <iostream>
#include <vector>

int start = -1;
int end   = -1;

enum DFSColors {
    BLACK,
    WHITE,
    GRAY
};

bool dfs(std::vector<DFSColors>& colors, std::vector<std::vector<int>>& tree, std::vector<int>& cycleP, int nodeId);
bool dfs(std::vector<DFSColors>& colors, std::vector<std::vector<int>>& tree, std::vector<int>& cycleP, int nodeId) {
    colors[nodeId] = GRAY;
    
    size_t curChildCnt = tree[nodeId].size();
    for (size_t i = 0; i < curChildCnt; i++) {
        int checkChild = tree[nodeId][i];

        if (colors[checkChild] == WHITE) {
            cycleP[checkChild] = nodeId;
            if (dfs(colors, tree, cycleP, checkChild)) return true;
        }
        if (colors[checkChild] == GRAY) {
            end     = nodeId;
            start   = checkChild;

            return true;
        }
    }

    colors[nodeId] = BLACK;
    return false;
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
    std::vector<int>       cycleP(N + 1, -1);
    for (int i = 1; i <= N; i++) {
        if (dfs(colors, paths, cycleP, i)) break;
    }
    
    if (start == -1) std::cout << "NO\n";
    else {
        std::cout << "YES\n";

        std::vector<int> temp;
        for (int i = end; i != start; i = cycleP[i]) temp.push_back(i);
        
        std::cout << start << ' ';
        for (long i = temp.size() - 1; i >= 0; i--) std::cout << temp[i] << ' ';
        std::cout << '\n';
    }

    return 0;
}