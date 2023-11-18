#include <vector>
#include <iostream>

//-------------------------------------------------------------------------

std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> readArguments();

void             topSort      (std::vector<int>& sorted, const std::vector<std::vector<int>>& graph, std::vector<bool>& used, const int vertInd);
std::vector<int> topSortDriver(const std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> graphs);

void                          dfs      (std::vector<int>& components, const std::vector<std::vector<int>>& graph, std::vector<bool>& used, const int vertInd);
std::vector<std::vector<int>> dfsDriver(const std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>>& graphs, const std::vector<int>& sorted);

void printAnswer(const std::vector<std::vector<int>>& components, const size_t N);

//-------------------------------------------------------------------------

std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> readArguments() {
    int N = 0, M = 0;
    std::cin >> N >> M;

    std::vector<std::vector<int>> graph  = std::vector(N, std::vector<int>());
    std::vector<std::vector<int>> graphT = std::vector(N, std::vector<int>());

    for (int i = 0; i < M; i++) {
        int p1 = 0, p2 = 0;
        std::cin >> p1 >> p2;

        p1--; p2--;

        graph [p1].push_back(p2);
        graphT[p2].push_back(p1);
    }

    return std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>>(graph, graphT);
}

//-------------------------------------------------------------------------

void topSort(std::vector<int>& sorted, const std::vector<std::vector<int>>& graph, std::vector<bool>& used, const int vertInd) {
    used[vertInd] = true;

    size_t graphSize = graph[vertInd].size();
    for (int i = 0; i < graphSize; i++) {
        if (!used[graph[vertInd][i]]) topSort(sorted, graph, used, graph[vertInd][i]);
    }

    sorted.push_back(vertInd);
}

std::vector<int> topSortDriver(const std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> graphs) {
    size_t N = graphs.first.size();

    std::vector<bool> used   = std::vector<bool>(N, false);
    std::vector<int>  sorted = std::vector<int>();

    for (int i = 0; i < N; i++) {
        if (!used[i]) topSort(sorted, graphs.first, used, i);
    }

    return sorted;
}

//-------------------------------------------------------------------------

void dfs(std::vector<int>& components, const std::vector<std::vector<int>>& graph, std::vector<bool>& used, const int vertInd) {
    used[vertInd] = true;
    components.push_back(vertInd);

    size_t graphSize = graph[vertInd].size();
    for (size_t i = 0; i < graphSize; i++) {
        if (!used[graph[vertInd][i]]) dfs(components, graph, used, graph[vertInd][i]);
    }

}

std::vector<std::vector<int>> dfsDriver(const std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>>& graphs, const std::vector<int>& sorted) {
    size_t N = graphs.first.size();

    std::vector<bool> used = std::vector(N, false);

    int componentCount  = 0;
    std::vector<std::vector<int>> components = std::vector<std::vector<int>>();

    for (int i = 0; i < N; i++) {
        int vertId = sorted[N - i - 1];
        if (!used[vertId]) {
            components.push_back(std::vector<int>());

            dfs(components[componentCount], graphs.second, used, vertId);
            componentCount++;
        }
    }

    return components;
}

//-------------------------------------------------------------------------

void printAnswer(const std::vector<std::vector<int>>& components, const size_t N) {
    size_t componentCnt = components.size();

    std::cout << componentCnt << '\n';

    for (int i = 0; i < N; i++) {
        for (size_t j = 0; j < componentCnt; j++) {
            for (size_t k = 0; k < components[j].size(); k++) {
                if (components[j][k] == i) std::cout << j + 1 << ' ';
            }
        }
    }

    std::cout << '\n';
}

//-------------------------------------------------------------------------

int main() {
    std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> graphs = readArguments();
    std::vector<int> sorted = topSortDriver(graphs);
    std::vector<std::vector<int>> components = dfsDriver(graphs, sorted);

    printAnswer(components, graphs.first.size());

    return 0;
}