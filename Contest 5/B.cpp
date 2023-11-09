#include <vector>
#include <iostream>
#include <algorithm>

void dfs(std::vector<std::vector<int>>& clones, int cloneId, std::vector<bool>& used, std::vector<int>& compArr);
void printComponents(std::vector<std::vector<int>>& clones, int N);

void dfs(std::vector<std::vector<int>>& clones, int cloneId, std::vector<bool>& used, std::vector<int>& compArr) {
    used[cloneId] = true;
    compArr.push_back(cloneId);

    size_t clonesCnt = clones[cloneId].size();
    for (size_t i = 0; i < clonesCnt; i++) {
        int dirClone = clones[cloneId][i];
        if (!used[dirClone]) dfs(clones, dirClone, used, compArr);
    }
}

void printComponents(std::vector<std::vector<int>>& clones, int N) {
    std::vector<bool> used = std::vector<bool>(N, false);
    std::vector<std::vector<int>> compArr;
    int count = 0;

    for (size_t i = 0; i < N; i++) {
        if (!used[i]) {
            compArr.push_back(std::vector<int>());

            dfs(clones, i, used, compArr[count++]);
        }
    }

    std::cout << count << '\n';
    for (int i = 0; i < count; i++) {
        std::cout << compArr[i].size() << '\n';

        std::sort(compArr[i].begin(), compArr[i].end());

        for (size_t j = 0; j < compArr[i].size(); j++) {
            std::cout << compArr[i][j] + 1 << ' ';
        }
        std::cout << '\n';
    }
}

int main() {

    int N = 0, M = 0;
    std::cin >> N >> M;

    std::vector<std::vector<int>> clones(N, std::vector<int>());

    for (int i = 0; i < N; i++) {
        clones[i].push_back(i);
    }

    for (int i = 0; i < M; i++) {
        int p1 = 0, p2 = 0;
        std::cin >> p1 >> p2;

        clones[p1 - 1].push_back(p2 - 1);
        clones[p2 - 1].push_back(p1 - 1);
    }

    printComponents(clones, N);

    return 0;
}