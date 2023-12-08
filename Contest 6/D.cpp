#include <bits/stdc++.h>

const long MAX_VAL = 30000;

struct Trio {
    long vert1;
    long vert2;
    long weight;

    explicit Trio() {}
    explicit Trio(long _vert1, long _vert2, long _weight) : vert1(_vert1), vert2(_vert2), weight(_weight) {}
};

class Graph {
private:
    std::vector<Trio> data;
    std::vector<long> dist;

public:
    explicit Graph(size_t size) {
        dist.resize(size, MAX_VAL);
    }

    void add(long _value1, long _value2, long _weight) {
        data.push_back(Trio(_value1, _value2, _weight)); 
        // data.push_back(Trio(_value2, _value1, _weight)); 
    }

    void dijkstra(int vert) {
        dist[vert] = 0;
        while (true) {
            bool has_changed = false;

            for (size_t i = 0; i < data.size(); i++) {
                if (dist[data[i].vert1] < MAX_VAL) {
                    if (dist[data[i].vert1] + data[i].weight < dist[data[i].vert2]) {
                        dist[data[i].vert2] = dist[data[i].vert1] + data[i].weight;
                        has_changed = true;
                    }
                }
            }

            if (!has_changed) break;
        }
        
    }

    void print() {
        for (size_t i = 0; i < dist.size(); i++) {
            std::cout << dist[i] << ' ';
        }
        std::cout << '\n';
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0); std::cout.tie(0);

    int N = 0, M = 0;
    std::cin >> N >> M;
    Graph temp(N);

    for (int j = 0; j < M; j++) {
        int val1, val2, w;
        std::cin >> val1 >> val2 >> w;

        temp.add(val1 - 1, val2 - 1, w);
    }

    temp.dijkstra(0);
    temp.print();

    return 0;
}