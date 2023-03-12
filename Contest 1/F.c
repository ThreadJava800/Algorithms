#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

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

int main() {
    int n = 0;
    INPUT_CHECK(scanf("%d", &n), 1);

    int* arr  = (int*) calloc(n, sizeof(int));
    int* pref = (int*) calloc(n, sizeof(int));
    int* post = (int*) calloc(n, sizeof(int));
    ON_ERROR(!arr, "Unable to alloc", -1);

    int locMin = 1000000000 + 1;
    for (int i = 0; i < n; i++) {
        INPUT_CHECK(scanf("%d", arr + i), 1);

        if (locMin > arr[i]) locMin = arr[i];
        pref[i] = locMin;
    } 

    locMin = 1000000000 + 1;
    for (int i = n - 1; i >= 0; i--) {
        if (locMin > arr[i]) locMin = arr[i];
        post[i] = locMin;
    }

    // printf("\n");
    // for (int i = 0; i < n; i++) {
    //     printf("%d ", pref[i]);
    // }
    // printf("\n");
    // for (int i = 0; i < n; i++) {
    //     printf("%d ", post[i]);
    // }
    // printf("\n");

    int q = 0;
    INPUT_CHECK(scanf("%d", &q), 1);

    for (int i = 0; i < q; i++) {
        int v1 = 0, v2 = 0;
        INPUT_CHECK(scanf("%d %d", &v1, &v2), 2);

        if (pref[v1 - 1] > post[v2 - 1]) printf("%d\n", post[v2 - 1]);
        else                     printf("%d\n", pref[v1 - 1]);
    }

    return 0;
}