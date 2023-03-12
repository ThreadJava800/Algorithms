#include "stdio.h"
#include "malloc.h"
#include "string.h"
#include "stdlib.h"

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


int comparator(const void *i, const void *j) {
    return *((int*) i) - *((int*) j);
}

int main() {
    int n = 0, m = 0, l = 0;
    INPUT_CHECK(scanf("%d %d %d", &n, &m, &l), 3);
    int* A = (int*) calloc(n * l, sizeof(int));
    int* B = (int*) calloc(m * l, sizeof(int));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < l; j++) {
            INPUT_CHECK(scanf("%d", A + l * i + j), 1);
        }
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < l; j++) {
            INPUT_CHECK(scanf("%d", B + l * i + j), 1);
        }
    }

    int q = 0;
    INPUT_CHECK(scanf("%d", &q), 1);

    for (int i = 0; i < q; i++) {
        int val1 = 0, val2 = 0;
        INPUT_CHECK(scanf("%d %d", &val1, &val2), 2);

        int mini = 100000 + 1;
        int aItr = 0, bItr = l - 1, prevPivot = 0;
        while (1) {
            int pivot = (aItr + bItr) / 2;

            // printf("\nVALS: %d %d\n", A[(val1 - 1) * l + pivot], B[(val2 - 1) * l + pivot]);
            if (A[(val1 - 1) * l + pivot] == B[(val2 - 1) * l + pivot]) {
                printf("%d\n", pivot + 1);
                break;
            }

            if (pivot == 0 || pivot == l - 1) {
                if (A[(val1 - 1) * l + pivot] >= B[(val2 - 1) * l + pivot]) {
                    if (mini >= A[(val1 - 1) * l + pivot]) {
                        printf("%d\n", pivot + 1);
                        break;
                    } else {
                        printf("%d\n", prevPivot + 1);
                        break;
                    }
                }
                else {
                    if (mini >= B[(val2 - 1) * l + pivot]) {
                        printf("%d\n", pivot + 1);
                        break;
                    } else {
                        printf("%d\n", prevPivot + 1);
                        break;
                    }
                }
            }

            if (A[(val1 - 1) * l + pivot] <= B[(val2 - 1) * l + pivot]) {
                if (mini >= B[(val2 - 1) * l + pivot]){
                    mini = B[(val2 - 1) * l + pivot];
                    prevPivot = pivot;
                } else {
                    printf("%d\n", prevPivot + 1);
                    break;
                }
                aItr = pivot + 1;
            } else {
                if (mini >= A[(val1 - 1) * l + pivot]){
                    mini = A[(val1 - 1) * l + pivot];
                    prevPivot = pivot;
                } else {
                    printf("%d\n", prevPivot + 1);
                    break;
                }
                bItr = pivot - 1;
            }
        }
    }

    return 0;
}