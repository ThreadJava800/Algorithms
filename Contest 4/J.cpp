#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#define ON_ERROR(expr, errStr, retVal) {                 \
    if (expr) {                                           \
        fprintf(stderr, "FATAL ERROR: %s\n", errStr);      \
        return retVal;                                      \
    }                                                        \
}                                                             \


// backtrace indexing = string index + 1
// so len(str1) = n - 1 and len(str2) = m - 1
size_t buildBacktraceArr(std::string& str1, std::string& str2, size_t** backtraceArr, size_t n, size_t m);

size_t buildBacktraceArr(std::string& str1, std::string& str2, size_t** backtraceArr, size_t n, size_t m) {
    ON_ERROR(!backtraceArr, "Pointer was nullptr", 0);

    size_t str1Len = n - 1;
    size_t str2Len = m - 1;

    for (size_t i = 0; i < str1Len; i++)
        backtraceArr[i][0] = 0;

    for (size_t j = 0; j < str2Len; j++)
        backtraceArr[0][j] = 0;

    for (size_t i = 1; i < str1Len + 1; i++) {
        for (size_t j = 1; j < str2Len + 1; j++) {
            if (str1[i - 1] == str2[j - 1])
                backtraceArr[i][j] = backtraceArr[i - 1][j - 1] + 1;
            else
                backtraceArr[i][j] = std::max(backtraceArr[i][j - 1], backtraceArr[i - 1][j]);
        }
    }

    return backtraceArr[str1Len][str2Len];
}

int main() {
    std::string str1 = "";
    std::string str2 = "";

    std::cin >> str1;
    std::cin >> str2;

    size_t n = str1.length();
    size_t m = str2.length();

    size_t** backTrace = new size_t*[n + 1];
    for (size_t i = 0; i <= n; i++) {
        backTrace[i] = new size_t[m + 1];
    }

    std::cout << buildBacktraceArr(str1, str2, backTrace, n + 1, m + 1) << '\n';

    return 0;
}