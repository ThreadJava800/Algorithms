#include <iostream>
#include <vector>

static const int PLUS_INF = 1e8;

using ivec2 = std::vector<std::vector<int>>;

void  getInput                   (const size_t n, const size_t m, std::vector<int>& coords);
void  generateDistanceArray      (std::vector<int>& d, const std::vector<int>& coords);
void  generateDistancesDPArray   (std::vector<int>* d, const std::vector<int>& coords,       ivec2& distancesDPArr, const size_t m);
void  calculateInfluencePoints   (std::vector<int>& d, const std::vector<int>& coords, const ivec2& distancesDPArr, const size_t m);
void  printInfluencePoints       (const std::vector<int>& d, const std::vector<int>& coords, const size_t m);
void  findAndPrintInfluencePoints(const size_t n, const size_t m);

void getInput(const size_t n, const size_t m, std::vector<int>& coords) {
    for (size_t i = 0; i < n; i++)
        std::cin >> coords[i];
}

void generateDistanceArray(std::vector<int>& d, const std::vector<int>& coords) {
    size_t coordsSize = coords.size();
    for (int i = 0; i < coordsSize; i++) {
        int sum = 0;
        for (int j = 0; j <= i - 1; j++) {
            sum += coords[i] - coords[j];
        }
        d[i] = sum;
    }
}

void generateDistancesDPArray(std::vector<int>* d, const std::vector<int>& coords, ivec2& distancesDPArr, const size_t m) {
    if (!d) return;

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
                if (summa + (*d)[j] < kthDistances[i]) {
                    kthDistances[i] = summa + (*d)[j];
                    distancesDPArr[i][k] = j;
                }
            }
        }
        *d = kthDistances;
    }
}

void calculateInfluencePoints(std::vector<int>& d, const std::vector<int>& coords, const ivec2& distancesDPArr, const size_t m) {
    size_t coordsSize = coords.size();

    int mini      = PLUS_INF;
    int rightBase = 0;
    for (int i = 0; i < coordsSize; i++) {
        int summa = d[i];

        for (int j = i + 1; j < coordsSize; j++) 
            summa += (coords[j] - coords[i]);

        if (summa < mini) {
            mini = summa;
            rightBase = i;
        }
    }

    std::cout << mini << '\n';

    int k = m - 1;
    while (k >= 0)
    {
        d[k] = rightBase;
        rightBase = distancesDPArr[rightBase][k];
        k--;
    }
}

void printInfluencePoints(const std::vector<int>& d, const std::vector<int>& coords, const size_t m) {
    for (int i = 0; i < m; i++) {
        std::cout << coords[d[i]] << ' ';
    }
}

void findAndPrintInfluencePoints(const size_t n, const size_t m) {
    std::vector<int> coords(n, 0);
    std::vector<int> d(n, 0);

    ivec2 distancesDPArr = std::vector<std::vector<int>>();
    for (size_t i = 0; i < n; i++) {
        distancesDPArr.push_back(std::vector<int>(m));
    }

    getInput(n, m, coords);

    generateDistanceArray(d, coords);
    generateDistancesDPArray(&d, coords, distancesDPArr, m);
    calculateInfluencePoints(d, coords, distancesDPArr, m);
    printInfluencePoints(d, coords, m);
}

int main() {
    size_t n = 0, m = 0;
    std::cin >> n >> m;

    findAndPrintInfluencePoints(n, m);

    return 0;
}