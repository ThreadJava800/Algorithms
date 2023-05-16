#include <limits.h>
#include <stdio.h>
#include <stdint.h>

#define false 0
#define true 1

const size_t MAX_STR_SIZE = 5e4 + 1;
const int    HASH_MOD     = 33554432 - 1; // 2^25 - 1
const int    POLYNOMIAL_P = 7;

size_t polynomialHash(const char *string, size_t patternLen);
short cmpSubs(const char *origString, size_t origStart, const char *pattern, size_t patLen);
void printSubstrOccur(const char *origString, size_t origSize, const char *substr, size_t substrSize);

#define INPUT_CHECK(readFunc, res) {        \
    int factRes = readFunc;                  \
    if (factRes != res) abort();              \
}                                              \


#define ON_ERROR(expr, errStr, retVal) {                 \
    if (expr) {                                           \
        fprintf(stderr, "FATAL ERROR: %s\n", errStr);      \
        return retVal;                                      \
    }                                                        \
}                                                             \

size_t polynomialHash(const char *string, size_t patternLen) {
    ON_ERROR(!string, "Nullptr", HASH_MOD);

    size_t hash = 0;
    size_t p    = 1;

    for (size_t i = 0; i < patternLen; i++) {
        hash = (hash + ((size_t)string[patternLen - i - 1] * p) % HASH_MOD) % HASH_MOD;
        p    = (p * POLYNOMIAL_P)     % HASH_MOD;
    }
    
    return hash;
}

short cmpSubs(const char *origString, size_t origStart, const char *pattern, size_t patLen) {
    ON_ERROR(!origString || !pattern, "Nullptr", false);

    for (size_t i = 0; i < patLen; i++) {
        if (pattern[i] != origString[origStart + i]) return false;
    }

    return true;
}

void printSubstrOccur(const char *origString, size_t origSize, const char *substr, size_t substrSize) {
    ON_ERROR(!origString || !substr, "Nullptr",);

    if (substrSize > origSize) return;

    size_t origHash   = polynomialHash(origString, substrSize);
    size_t subStrHash = polynomialHash(substr,     substrSize);

    // p = POLYNOMIAL_P ^ (substrSize - 1)
    size_t p = 1;
    for (size_t i = 0; i < substrSize - 1; i++) {
        p = (POLYNOMIAL_P * p) % HASH_MOD;
    }

    for (size_t i = 0; i <= origSize - substrSize; i++) {
        if (origHash == subStrHash) {
            if (cmpSubs(origString, i, substr, substrSize)) printf("%lu\n", i);
        }

        if (i == origSize - substrSize) break;

        // printf("BEFORE: %lu %lu\n", origHash, subStrHash);

        size_t subVal = ((size_t) origString[i] * p) % HASH_MOD;

        if (subVal > origHash) origHash = (origHash - subVal + HASH_MOD) % HASH_MOD;
        else                   origHash = (origHash - subVal)            % HASH_MOD;

        origHash = (origHash * POLYNOMIAL_P)                             % HASH_MOD;
        origHash = (origHash + (size_t) origString[i + substrSize])               % HASH_MOD;

        // printf("AFTER: %lu %lu\n", origHash, subStrHash);
    }
}

int main() {
    char origBuffer[MAX_STR_SIZE];
    char pattern   [MAX_STR_SIZE];

    int origSize    = 0;
    int patternSize = 0;

    scanf("%s%n", origBuffer, &origSize);
    scanf("%s%n", pattern, &patternSize);

    // printf("%s\n%s\n%d %d\n\n", origBuffer, pattern, origSize, patternSize - 1);
    printSubstrOccur(origBuffer, (size_t) origSize, pattern, (size_t) patternSize - 1);

    return 0;
}