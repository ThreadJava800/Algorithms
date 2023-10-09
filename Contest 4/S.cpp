#include <iostream>
#include <vector>

inline bool contains(size_t mask, int check);
void makeDP (std::vector<std::vector<bool>>* dp, size_t N);
void makeDP2(std::vector<std::vector<bool>>* dp, std::vector<std::vector<size_t>>* dp2, size_t M);
size_t getCount(std::vector<std::vector<bool>>* dp, std::vector<std::vector<size_t>>* dp2, size_t M);

inline bool contains(size_t mask, int check) {
    return mask & (1 << check);
}

void makeDP(std::vector<std::vector<bool>>* dp, size_t N) {
    if (!dp) return;

    for (size_t i = 0; i < dp->size(); i++) {
        for (size_t j = 0; j < dp->size(); j++) {
            bool cell = true;

            for (size_t k = 0; k < N - 1; k++) {
                int checkSum = contains(i, k) + contains(i, k + 1) + contains(j, k) + contains(j, k + 1);
                
                cell = !(checkSum == 0 || checkSum == 4);
                if (!cell) break;
            }

            (*dp)[i][j] = cell;
        }
    }
}

void makeDP2(std::vector<std::vector<bool>>* dp, std::vector<std::vector<size_t>>* dp2, size_t M) {
    if (!dp || !dp2) return;

    for (size_t i = 0; i < dp->size(); i++) (*dp2)[0][i] = 1;

    for (size_t i = 1; i < M; i++) {
        for (size_t j = 0; j < dp->size(); j++) {
            for (size_t k = 0; k < dp->size(); k++) {
                if ((*dp)[j][k]) (*dp2)[i][k] += (*dp2)[i - 1][j];
            }
        }
    }
}

size_t getCount(std::vector<std::vector<bool>>* dp, std::vector<std::vector<size_t>>* dp2, size_t M) {
    if (!dp || !dp2) return 0;

    size_t count = 0;

    for (size_t i = 0; i < dp->size(); i++) count += (*dp2)[M - 1][i];

    return count;
}

int main() {
    int N = 0, M = 0;
    std::cin >> N >> M;

    if (N > M) {
        int temp = M;
        M = N;
        N = temp;
    }

    std::vector<std::vector<bool>>   dp(1 << N, std::vector<bool>(1 << N, true));
    std::vector<std::vector<size_t>> dp2(M, std::vector<size_t>(1 << N, 0));
    makeDP (&dp, N);
    makeDP2(&dp, &dp2, M);

    size_t variants = getCount(&dp, &dp2, M);

    std::cout << variants << '\n';

    return 0;
}