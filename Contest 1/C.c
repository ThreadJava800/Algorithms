#include "stdio.h"
#include "stdlib.h"


#define INPUT_CHECK(readFunc, res) {        \
    long long int factRes = readFunc;                  \
    if (factRes != res) abort();              \
}                                              \

long long int canLight(long long int* array, long long int size, long long int cnt, long long int k) {
    long long int start = array[0], localK = 1;
    for (long long int i = 0; i < size; i++) {
        if (array[i] - start <= cnt) continue;

        localK++;
        start = array[i];

        if (localK > k) return 0;
    }

    return 1;
}

int comparator(const void *i, const void *j) {
    return *((int*) i) - *((int*) j);
}

int main() {
    long long int n = 0, k = 0;
    INPUT_CHECK(scanf("%lld %lld", &n, &k), 2);

    long long int* array = (long long int*) calloc(n , sizeof(long long int));
    for (long long int i = 0; i < n; i++) {
        INPUT_CHECK(scanf("%lld", array + i), 1);
    }

    qsort(array, n, sizeof(long long int), comparator);

    long long int i = -1, j = array[n - 1] - array[0] + 1;
    while (i < j - 1) {
        long long int mid = (i + j) / 2;

        if (canLight(array, n, mid, k)) {
            j = mid;
        }
        else {
            i = mid;
        }
    }

     printf("%lld", j);

    return 0;
}