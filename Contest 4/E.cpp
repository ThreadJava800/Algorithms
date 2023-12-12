#include <iostream>
#include <vector>

static const int PLUS_INF = 1e8;

using ivec2 = std::vector<std::vector<int>>;

void             getInput                (const size_t n, const size_t m, std::vector<int>& coords);
void             generateDistanceArray   (std::vector<int>& dist, const std::vector<int>& coords);
void             generateDistancesDPArray(std::vector<int>* dist, const std::vector<int>& coords,       ivec2& distancesDPArr, const size_t m);
int              calculateInfluencePoints(std::vector<int>& dist, const std::vector<int>& coords, const ivec2& distancesDPArr, const size_t m);
void             printVector             (const std::vector<int>& vec);
std::vector<int> createInfluencePointList(const std::vector<int>& dist, const std::vector<int>& coords, const size_t m);
std::vector<int> getInfluencePoints      (const size_t n, const size_t m);

void getInput(const size_t n, const size_t m, std::vector<int>& coords) {
    for (size_t i = 0; i < n; i++)
        std::cin >> coords[i];
}

void generateDistanceArray(std::vector<int>& dist, const std::vector<int>& coords) {
    size_t coordsSize = coords.size();
    for (int i = 0; i < coordsSize; i++) {
        int sum = 0;
        for (int j = 0; j <= i - 1; j++) {
            sum += coords[i] - coords[j];
        }
        dist[i] = sum;
    }
}

void generateDistancesDPArray(std::vector<int>* dist, const std::vector<int>& coords, ivec2& distancesDPArr, const size_t m) {
    if (!dist) return;

    size_t coordsSize = coords.size();
    std::vector<int> kthDistances(coordsSize, 0);

    for (int k = 1; k < m; k++) {
        for (int i = 0; i < coordsSize; i++) {
            kthDistances[i] = PLUS_INF;

            int r = i;
            int summa = 0;

            for (int j = i - 1; j >= 0; j--) {
                while((r > j) && (coords[i] - coords[r - 1] < coords[r - 1] - coords[j])) {
                    r--;
                    summa = summa - (coords[r] - coords[j + 1]) + (coords[i] - coords[r]);
                }

                summa += (r - j - 1) * (coords[j + 1] - coords[j]);
                if (summa + (*dist)[j] < kthDistances[i]) {
                    kthDistances[i] = summa + (*dist)[j];
                    distancesDPArr[i][k] = j;
                }
            }
        }
        *dist = kthDistances;
    }
}

int calculateInfluencePoints(std::vector<int>& dist, const std::vector<int>& coords, const ivec2& distancesDPArr, const size_t m) {
    size_t coordsSize = coords.size();

    int minSumma  = PLUS_INF;
    int rightBase = 0;
    for (int i = 0; i < coordsSize; i++) {
        int summa = dist[i];

        for (int j = i + 1; j < coordsSize; j++) 
            summa += (coords[j] - coords[i]);

        if (summa < minSumma) {
            minSumma = summa;
            rightBase = i;
        }
    }

    int k = m - 1;
    while (k >= 0)
    {
        dist[k] = rightBase;
        rightBase = distancesDPArr[rightBase][k];
        k--;
    }

    return minSumma;
}

void printVector(const std::vector<int>& vec) {
    for (int i = 0; i < vec.size(); i++) {
        std::cout << vec[i] << ' ';
    }
}

std::vector<int> createInfluencePointList(const std::vector<int>& dist, const std::vector<int>& coords, const size_t m) {
    std::vector<int> influencePoints;

    for (int i = 0; i < m; i++) {
        influencePoints.push_back(coords[dist[i]]);
    }

    return influencePoints;
}

std::vector<int> getInfluencePoints(const size_t n, const size_t m) {
    std::vector<int> coords(n, 0);
    std::vector<int> dist(n, 0);

    ivec2 distancesDPArr = std::vector<std::vector<int>>();
    for (size_t i = 0; i < n; i++) {
        distancesDPArr.push_back(std::vector<int>(m));
    }

    getInput(n, m, coords);

    generateDistanceArray(dist, coords);
    generateDistancesDPArray(&dist, coords, distancesDPArr, m);
    int minSumma = calculateInfluencePoints(dist, coords, distancesDPArr, m);
    std::cout << minSumma << '\n';

    return createInfluencePointList(dist, coords, m);
}

int main() {
    size_t n = 0, m = 0;
    std::cin >> n >> m;

    auto points = getInfluencePoints(n, m);
    printVector(points);

    return 0;
}