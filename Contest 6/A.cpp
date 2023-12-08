#include <bits/stdc++.h>

const long MAX_VAL = 2009000999;

struct Trio {
    long vert1;
    long vert2;
    long weight;

    explicit Trio(long _vert1, long _vert2, long _weight) : vert1(_vert1), vert2(_vert2), weight(_weight) {}
};

class Graph {
private:
    std::vector<std::vector<std::pair<size_t, size_t>>> data;
    std::vector<long>              result;
    std::vector<long> dist;
    std::vector<bool> used;

public:
    explicit Graph(size_t size) {
        data.resize(size);
        dist.resize(size, MAX_VAL);
        used.resize(size, false);
    }

    void add(long _value1, long _value2, long _weight) {
        data[_value1].push_back({_value2, _weight}); 
        data[_value2].push_back({_value1, _weight}); 
    }

    void dijkstra(int vert) {
        std::priority_queue<std::pair<long, long>> optim;
        optim.push(std::make_pair(vert, 0));

        dist[vert] = 0;

        while (!optim.empty()) {
            int check_vert = optim.top().first;
            optim.pop();

            for (auto j : data[check_vert]) {
                if (dist[check_vert] + j.second < dist[j.first]) {
                    dist[j.first] = dist[check_vert] + j.second;
                    optim.push(std::make_pair(j.first, dist[j.first]));
                }
            }
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

    int K = 0, N = 0, M = 0;
    std::cin >> K;

    for (int i = 0; i < K; i++) {
        std::cin >> N >> M;
        Graph temp(N);

        for (int j = 0; j < M; j++) {
            int val1, val2, w;
            std::cin >> val1 >> val2 >> w;

            temp.add(val1, val2, w);
        }

        int vvv = 0;
        std::cin >> vvv;
        temp.dijkstra(vvv);
        temp.print();
    }



    return 0;
}