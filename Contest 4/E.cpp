#include <iostream>
#include <vector>

static const int PLUS_INF = 1e8;

void generateD  (std::vector<int>* d, std::vector<int>& coords);
void generateDP (std::vector<int>* d, std::vector<int>& coords, int** dp, int m);
void printAnswer(std::vector<int>* d, std::vector<int>& coords, int** dp, int m);

void generateD(std::vector<int>* d, std::vector<int>& coords) {
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

void generateDP(std::vector<int>* d, std::vector<int>& coords, int** dp, int m) {
    if (!d || !dp) return;

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
                    dp[i][k] = j;
                }
            }
        }
        *d = temp;
    }
}

void printAnswer(std::vector<int>* d, std::vector<int>& coords, int** dp, int m) {
    if (!d || !dp) return;

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
        rightBase = dp[rightBase][k];
        k--;
    }
    
    for (int i = 0; i < m; i++) {
        std::cout << coords[(*d)[i]] << ' ';
    }
}

int main() {
    int n = 0, m = 0;
    std::cin >> n >> m;

    std::vector<int> coords(n, 0);
    std::vector<int> d(n, 0);
    int** dp = new int*[n];
    for (int i = 0; i < n; i++) {
        dp[i] = new int[n];
        for (int j = 0; j < n ;j++) {
            dp[i][j] = 0;
        }
    }

    for (int i = 0; i < n; i++)
        std::cin >> coords[i];

    generateD  (&d, coords);
    generateDP (&d, coords, dp, m);
    printAnswer(&d, coords, dp, m);
    
    for (int i = 0; i < n; i++) {
        delete[] dp[i];
    }
    delete[] dp;

    return 0;
}