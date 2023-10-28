#include <vector>
#include <iostream>

using intDim2 = std::vector<std::vector<int>>;

//-------------------------------------------------------------------------

std::pair<intDim2*, intDim2*> readArguments();

void              topSort      (std::vector<int>& sorted, intDim2& graph, std::vector<bool>& used, int vertInd);
std::vector<int>* topSortDriver(std::pair<intDim2*, intDim2*> graphs);

void    dfs       (std::vector<int>& components, intDim2& graph, std::vector<bool>& used, int vertInd);
intDim2* dfsDriver(std::pair<intDim2*, intDim2*> graphs, std::vector<int>& sorted);

void printAnswer(intDim2& components, size_t N);

void freeMemory(std::pair<intDim2*, intDim2*> graphs, intDim2* components, std::vector<int>* sorted);

//-------------------------------------------------------------------------

std::pair<intDim2*, intDim2*> readArguments() {
    int N = 0, M = 0;
    std::cin >> N >> M;

    intDim2* graph  = new std::vector(N, std::vector<int>());
    intDim2* graphT = new std::vector(N, std::vector<int>());

    for (int i = 0; i < M; i++) {
        int p1 = 0, p2 = 0;
        std::cin >> p1 >> p2;

        p1--; p2--;

        (*graph) [p1].push_back(p2);
        (*graphT)[p2].push_back(p1);
    }

    return std::pair<intDim2*, intDim2*>(graph, graphT);
}

//-------------------------------------------------------------------------

void topSort(std::vector<int>& sorted, intDim2& graph, std::vector<bool>& used, int vertInd) {
    used[vertInd] = true;

    size_t graphSize = graph[vertInd].size();
    for (int i = 0; i < graphSize; i++) {
        if (!used[graph[vertInd][i]]) topSort(sorted, graph, used, graph[vertInd][i]);
    }

    sorted.push_back(vertInd);
}

std::vector<int>* topSortDriver(std::pair<intDim2*, intDim2*> graphs) {
    size_t N = graphs.first->size();

    std::vector<bool> used   = std::vector<bool>(N, false);
    std::vector<int>* sorted = new std::vector<int>();

    for (int i = 0; i < N; i++) {
        if (!used[i]) topSort(*sorted, *graphs.first, used, i);
    }

    return sorted;
}

//-------------------------------------------------------------------------

void dfs(std::vector<int>& components, intDim2& graph, std::vector<bool>& used, int vertInd) {
    used[vertInd] = true;
    components.push_back(vertInd);

    size_t graphSize = graph[vertInd].size();
    for (size_t i = 0; i < graphSize; i++) {
        if (!used[graph[vertInd][i]]) dfs(components, graph, used, graph[vertInd][i]);
    }

}

intDim2* dfsDriver(std::pair<intDim2*, intDim2*> graphs, std::vector<int>& sorted) {
    size_t N = graphs.first->size();

    std::vector<bool> used = std::vector(N, false);

    int componentCount  = 0;
    intDim2* components = new std::vector<std::vector<int>>();

    for (int i = 0; i < N; i++) {
        int vertId = sorted[N - i - 1];
        if (!used[vertId]) {
            components->push_back(std::vector<int>());

            dfs((*components)[componentCount], *graphs.second, used, vertId);
            componentCount++;
        }
    }

    return components;
}

//-------------------------------------------------------------------------

void printAnswer(intDim2& components, size_t N) {
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

void freeMemory(std::pair<intDim2*, intDim2*> graphs, intDim2* components, std::vector<int>* sorted) {
    delete graphs.first; delete graphs.second;
    delete components;
    delete sorted;
}

//-------------------------------------------------------------------------

int main() {
    std::pair<intDim2*, intDim2*> graphs = readArguments();
    std::vector<int>* sorted = topSortDriver(graphs);
    intDim2* components = dfsDriver(graphs, *sorted);

    printAnswer(*components, graphs.first->size());

    freeMemory(graphs, components, sorted);

    return 0;
}