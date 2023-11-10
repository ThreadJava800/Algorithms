#include <iostream>
#include <vector>

static const int PLUS_INF = 1e8;

int** getInput                (int n, int m, std::vector<int>& coords);
void  generateDistanceArray   (std::vector<int>* d, std::vector<int>& coords);
void  generateDistancesDPArray(std::vector<int>* d, std::vector<int>& coords, int** distancesDPArr, int m);
void  printAnswer             (std::vector<int>* d, std::vector<int>& coords, int** distancesDPArr, int m);
void  freeMemory              (int** distancesDPArr, int n);

int** getInput(int n, int m, std::vector<int>& coords) {
    int** distancesDPArr = new int*[n];
    for (int i = 0; i < n; i++) {
        distancesDPArr[i] = new int[n];
        for (int j = 0; j < n ;j++) {
            distancesDPArr[i][j] = 0;
        }
    }

    for (int i = 0; i < n; i++)
        std::cin >> coords[i];

    return distancesDPArr;
}

void generateDistanceArray(std::vector<int>* d, std::vector<int>& coords) {
    if (!d) return;

    size_t coordsSize = coords.size();
    for (int i = 0; i < coordsSize; i++) {
        int sum = 0;
        for (int j = 0; j <= i - 1; j++) {
            sum += coords[i] - coords[j];
        }
        (*d)[i] = sum;
    }
}

void generateDistancesDPArray(std::vector<int>* d, std::vector<int>& coords, int** distancesDPArr, int m) {
    if (!d || !distancesDPArr) return;

    size_t coordsSize = coords.size();
    std::vector<int> temp(coordsSize, 0);

    for (int k = 1; k < m; k++) {
        for (int i = 0; i < coordsSize; i++) {
            temp[i] = PLUS_INF;

            int r = i;
            int summa = 0;

            for (int j = i - 1; j >= 0; j--) {
                while((r > j) && (coords[i] - coords[r - 1] < coords[r - 1] - coords[j])) {
                    r--;
                    summa = summa - (coords[r] - coords[j + 1]) + (coords[i] - coords[r]);
                }

                summa += (r - j - 1) * (coords[j + 1] - coords[j]);
                if (summa + (*d)[j] < temp[i]) {
                    temp[i] = summa + (*d)[j];
                    distancesDPArr[i][k] = j;
                }
            }
        }
        *d = temp;
    }
}

void printAnswer(std::vector<int>* d, std::vector<int>& coords, int** distancesDPArr, int m) {
    if (!d || !distancesDPArr) return;

    size_t coordsSize = coords.size();

    int mini      = PLUS_INF;
    int rightBase = 0;
    for (int i = 0; i < coordsSize; i++) {
        int summa = (*d)[i];

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
        (*d)[k] = rightBase;
        rightBase = distancesDPArr[rightBase][k];
        k--;
    }
    
    for (int i = 0; i < m; i++) {
        std::cout << coords[(*d)[i]] << ' ';
    }
}

void freeMemory(int** distancesDPArr, int n) {
    for (int i = 0; i < n; i++) {
        delete[] distancesDPArr[i];
    }
    delete[] distancesDPArr;
}

int main() {
    int n = 0, m = 0;
    std::cin >> n >> m;

    std::vector<int> coords(n, 0);
    std::vector<int> d(n, 0);

    int** distancesDPArr = getInput(n, m, coords);

    generateDistanceArray(&d, coords);
    generateDistancesDPArray(&d, coords, distancesDPArr, m);
    printAnswer(&d, coords, distancesDPArr, m);
    freeMemory(distancesDPArr, n);

    return 0;
}