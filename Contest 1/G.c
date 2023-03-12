#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <math.h>

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


typedef struct {
    int start;
    int end;
} Pair_t;

int count = 0;
int n = 0;

Pair_t* getPairs();
void mqsort(int l, int r, Pair_t* toSort);
void checker(Pair_t* toSort, Pair_t* result);
void printPairs(Pair_t* pairs);

Pair_t* getPairs() {
    INPUT_CHECK(scanf("%d", &n), 1);
    Pair_t* pairs = (Pair_t*) calloc(n, sizeof(Pair_t));

    for (int i = 0; i < n; i++) {
        int v1 = 0, v2 = 0;
        INPUT_CHECK(scanf("%d %d", &v1, &v2), 2);

        pairs[i].start = v1;
        pairs[i].end   = v2;
    }

    return pairs;
}

void mqsort(int l, int r, Pair_t* toSort) {
    ON_ERROR(l < 0 || r < 0 || l > r, "Incorrect borders!",);

    int i = l, j = r;
    int pivot = toSort[(i + j) / 2].start;

    while (i <= j) {

        while (toSort[i].start < pivot) {
            i++;
        }
        while (toSort[j].start > pivot) {
            j--;
        }

        if (i <= j) {
            Pair_t _temp = toSort[i];
            toSort[i] = toSort[j];
            toSort[j] = _temp;

            i++;
            j--;
        }
    }

    if (l < j) mqsort(l, j, toSort);
    if (i < r) mqsort(i, r, toSort);
}

void checker(Pair_t* toSort, Pair_t* result) {
    int curStart = toSort[0].start, curEnd = toSort[0].end;

    for (int i = 0; i < n - 1; i++) {
        //Pair_t cur  = toSort[i];
        Pair_t next = toSort[i + 1];

        if (curStart <= next.start && next.start <= curEnd) {
            if (next.end > curEnd) {
                curEnd = next.end;
            }
        } else {
            result[count].start = curStart;
            result[count].end   = curEnd;

            count++;
            curStart = next.start;
            curEnd   = next.end;
            
        }

        if (i == n - 2) {
            if (next.end > curEnd) {
                curEnd = next.end;
            }
            result[count].start = curStart;
            result[count].end   = curEnd;

            count++;
        }
    }
}

void printPairs(Pair_t* pairs) {
    printf("%d\n", count);
    for (int i = 0; i < count; i++) {
        printf("%d %d\n", pairs[i].start, pairs[i].end);
    }
}

int main() {
    Pair_t* pairs  = getPairs();
    if (n == 1) {
        count = 1;
        printPairs(pairs);
        return 0;
    }

    Pair_t* result = (Pair_t*) calloc(n, sizeof(Pair_t));
    mqsort(0, n - 1, pairs);
    checker(pairs, result);
    printPairs(result);

    return 0;
}