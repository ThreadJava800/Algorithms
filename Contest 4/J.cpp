#include <iostream>
#include <string>
#include <cmath>
#include <vector>

using lluvec2 = std::vector<std::vector<size_t>>;

enum BackTraceMode {
    FIRST  = 1,
    SECOND = 2
};

// backtrace indexing = string index + 1
// so len(str1) = n - 1 and len(str2) = m - 1
size_t buildBacktraceArr(std::string& str1, std::string& str2, lluvec2& backtraceArr, size_t n, size_t m);
void doBacktrace        (std::string& str1, std::string& str2, lluvec2& backtraceArr, std::vector<size_t>& meets, BackTraceMode mode);

size_t buildBacktraceArr(std::string& str1, std::string& str2, lluvec2& backtraceArr, size_t n, size_t m) {
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

void doBacktrace (std::string& str1, std::string& str2, lluvec2& backtraceArr, std::vector<size_t>& meets, BackTraceMode mode) {
    size_t i = str1.length();
    size_t j = str2.length();

    while (i > 0 && j > 0) {
        if (str1[i - 1] == str2[j - 1]) {
            if (mode == FIRST) meets.push_back(i - 1);
            else               meets.push_back(j - 1);
            i--;
            j--;
        }
        else if (backtraceArr[i][j - 1] > backtraceArr[i - 1][j]) {
            j--;
        }
        else i--;
    }
}

int main() {
    std::string str1 = "";
    std::string str2 = "";

    std::cin >> str1;
    std::cin >> str2;

    size_t n = str1.length();
    size_t m = str2.length();

    lluvec2 backTrace;
    for (size_t i = 0; i <= n; i++) {
        backTrace.push_back(std::vector<size_t>(m + 1));
    }

    std::cout << buildBacktraceArr(str1, str2, backTrace, n + 1, m + 1) << '\n';

    std::vector<size_t> vecStr;
    doBacktrace(str1, str2, backTrace, vecStr, FIRST);

    for (long i = long(vecStr.size()) - 1; i >= 0; i--) {
        std::cout << vecStr[size_t(i)] + 1 << ' ';
    }
    std::cout << '\n';

    vecStr.clear();
    doBacktrace(str1, str2, backTrace, vecStr, SECOND);
    for (long i = long(vecStr.size()) - 1; i >= 0; i--) {
        std::cout << vecStr[size_t(i)] + 1 << ' ';
    }

    return 0;
}