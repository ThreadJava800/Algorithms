#include <iostream>
#include <vector>

size_t getBaseCnt (std::vector<size_t>& houses, size_t divider);
size_t getBaseDist(std::vector<size_t>& houses, size_t m);
void   printBases (std::vector<size_t>& houses, size_t startInd, size_t maxDist);

size_t getBaseCnt(std::vector<size_t>& houses, size_t divider) {
    size_t baseCnt = 1, lastBase = houses[0];
    size_t houseCnt = houses.size();

    for (size_t i = 0; i < houseCnt; i++) {
        if (i - lastBase >= divider) {
            baseCnt++;
            lastBase = i;
        }
    }

    return baseCnt;
}

size_t getBaseDist (std::vector<size_t>& houses, size_t m) {
    size_t left = 0;    // min len
    size_t right = houses.size() - houses[0] + 1; // max len

    while (left < right)
    {   
        size_t mid     = left + (right - left) / 2;
        size_t baseCnt = getBaseCnt(houses, mid);

        if (baseCnt >= m) left  = mid + 1;
        else              right = mid;
    }
    
    return left;
}

void printBases(std::vector<size_t>& houses, size_t startInd, size_t maxDist) {
    std::vector<size_t> bases;
    size_t sumDist = 0;

    bases.push_back(houses[startInd]);

    size_t basesCnt = 1;
    size_t houseCnt = houses.size();
    for (size_t i = startInd + 1; i < houseCnt; i++) {
        if (houses[i] - bases[basesCnt - 1] > maxDist) {
            bases.push_back(houses[i]);
            basesCnt++;
        }
    }

    for (size_t i = 0; i < houseCnt; i++) {

    }

    std::cout << sumDist << '\n';
    for (size_t i = 0; i < basesCnt; i++)
        std::cout << bases[i] << ' ';
    std::cout << '\n';
}

int main() {
    size_t n = 0, m = 0;
    std::cin >> n >> m;

    std::vector<size_t> coords(n);

    for (size_t i = 0; i < n; i++)
        std::cin >> coords[i];

    size_t baseDist = getBaseDist(coords, m);
    size_t startInd = 0;

    // std::cout << baseDist << '\n';

    for (size_t i = 1; i < n; i++) {
        if (coords[i] - coords[0] > baseDist) {
            startInd = i - 1;
            break;
        }
    }

    // std::cout << startInd << '\n';

    printBases(coords, startInd, getBaseDist(coords, m));

    return 0;
}