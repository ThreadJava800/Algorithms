#include <vector>
#include <iostream>

class Graph {
private:
    std::vector<std::vector<int>> edges;
    std::vector<int>              sorted_vertexes;
    std::vector<std::vector<int>> strong_components;
    std::vector<std::vector<int>> edges_transposed;

public:
    explicit Graph(const int N) {
        edges           .resize(N);
        edges_transposed.resize(N);
    }

    void addEdge(int from, int to) {
        edges[from]         .push_back(to);
        edges_transposed[to].push_back(from);
    }

    void _topSort(std::vector<int>& sorted, std::vector<bool>& used, const int vertInd) {
        used[vertInd] = true;

        size_t graphSize = edges[vertInd].size();
        for (int i = 0; i < graphSize; i++) {
            if (!used[edges[vertInd][i]]) _topSort(sorted, used, edges[vertInd][i]);
        }

        sorted.push_back(vertInd);
    }

    void doTopSort() {
        size_t N = edges.size();

        std::vector<bool> used = std::vector<bool>(N, false);

        for (int i = 0; i < N; i++) {
            if (!used[i]) _topSort(sorted_vertexes, used, i);
        }
    }

    void _strongCompDFS(std::vector<int>& components, std::vector<bool>& used, const int vert_ind) {
        used[vert_ind] = true;
        components.push_back(vert_ind);

        size_t graph_size = edges_transposed[vert_ind].size();
        for (size_t i = 0; i < graph_size; i++) {
            if (!used[edges_transposed[vert_ind][i]]) _strongCompDFS(components, used, edges_transposed[vert_ind][i]);
        }
    }

    void findStrongComponents() {
        size_t N = edges.size();
        int component_count  = 0;

        std::vector<bool> used = std::vector(N, false);

        for (int i = 0; i < N; i++) {
            int vert_id = sorted_vertexes[N - i - 1];
            if (!used[vert_id]) {
                strong_components.push_back(std::vector<int>());

                _strongCompDFS(strong_components[component_count], used, vert_id);
                component_count++;
            }
        }
    }

    void calculateAndPrintStrongComponents() {
        doTopSort();
        findStrongComponents();

        size_t component_cnt = strong_components.size();

        std::cout << component_cnt << '\n';

        for (int i = 0; i < edges.size(); i++) {
            for (size_t j = 0; j < component_cnt; j++) {
                for (size_t k = 0; k < strong_components[j].size(); k++) {
                    if (strong_components[j][k] == i) std::cout << j + 1 << ' ';
                }
            }
        }

        std::cout << '\n';
    }
};

//-------------------------------------------------------------------------

Graph readArgumentsAndCreateGraph();

//-------------------------------------------------------------------------

Graph readArgumentsAndCreateGraph() {
    int N = 0, M = 0;
    std::cin >> N >> M;

    Graph graph(N);

    for (int i = 0; i < M; i++) {
        int p1 = 0, p2 = 0;
        std::cin >> p1 >> p2;

        p1--; p2--;

        graph.addEdge(p1, p2);
    }

    return graph;
}

//-------------------------------------------------------------------------

int main() {
    Graph graph = readArgumentsAndCreateGraph();
    graph.calculateAndPrintStrongComponents();

    return 0;
}