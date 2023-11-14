#include <iostream>
#include <vector>
#include <climits>

inline bool contains(const size_t mask, const int check);
void   makeAreaDP   (      std::vector<std::vector<bool>>* areaDP, const size_t N);
void   makeAreaDP2  (const std::vector<std::vector<bool>>* areaDP,       std::vector<std::vector<size_t>>* areaDP2, const size_t M);
size_t getCount     (const std::vector<std::vector<bool>>* areaDP, const std::vector<std::vector<size_t>>* areaDP2, const size_t M);

inline bool contains(const size_t mask, const int check) {
    return mask & (1 << check);
}

inline bool checkProfile(size_t profile1, size_t profile2, size_t profileLimit = SSIZE_MAX) {
    size_t greenMatch  = profile1 & profile2;
    size_t purpleMatch = (~profile1 & ~profile2) & (profileLimit - 1);

    return ((greenMatch & (greenMatch << 1)) == 0) && ((purpleMatch & (purpleMatch << 1)) == 0);
}

void makeAreaDP(std::vector<std::vector<bool>>* areaDP, const size_t N) {
    if (!areaDP) return;

    for (size_t i = 0; i < areaDP->size(); i++) {
        for (size_t j = 0; j < areaDP->size(); j++) {
            // bool cell = true;

            // for (size_t k = 0; k < N - 1; k++) {
            //     int checkSum = contains(i, k) + contains(i, k + 1) + contains(j, k) + contains(j, k + 1);
                
            //     cell = !(checkSum == 0 || checkSum == 4);
            //     if (!cell) break;   
            // }

            (*areaDP)[i][j] = checkProfile(i, j, areaDP->size());
        }
    }
}

void makeAreaDP2(const std::vector<std::vector<bool>>* areaDP, std::vector<std::vector<size_t>>* areaDP2, const size_t M) {
    if (!areaDP || !areaDP2) return;

    for (size_t i = 0; i < areaDP->size(); i++) (*areaDP2)[0][i] = 1;

    for (size_t i = 1; i < M; i++) {
        for (size_t j = 0; j < areaDP->size(); j++) {
            for (size_t k = 0; k < areaDP->size(); k++) {
                if ((*areaDP)[j][k]) (*areaDP2)[i][k] += (*areaDP2)[i - 1][j];
            }
        }
    }
}

size_t getCount(const std::vector<std::vector<bool>>* areaDP, const std::vector<std::vector<size_t>>* areaDP2, const size_t M) {
    if (!areaDP || !areaDP2) return 0;

    size_t count = 0;

    for (size_t i = 0; i < areaDP->size(); i++) count += (*areaDP2)[M - 1][i];

    return count;
}

size_t proceedAlgorithm(size_t N, size_t M) {
    if (N > M) std::swap(N, M);

    std::vector<std::vector<bool>>   areaDP (1 << N, std::vector<bool>(1 << N, true));
    std::vector<std::vector<size_t>> areaDP2(M, std::vector<size_t>(1 << N, 0));
    makeAreaDP (&areaDP, N);
    makeAreaDP2(&areaDP, &areaDP2, M);

    size_t variants = getCount(&areaDP, &areaDP2, M);

    return variants;
}

int main() {
    size_t N = 0, M = 0;
    std::cin >> N >> M;

    std::cout << proceedAlgorithm(N, M) << '\n';

    return 0;
}