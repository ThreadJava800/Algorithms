#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

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


size_t newValue(size_t a1, size_t a2);
int qPart(size_t* arr, int l, int r);
size_t search(size_t* arr, int k, int l, int r);

size_t newValue(size_t a1, size_t a2) {
    return (a2 * 123 + a1 * 45) % (10000000 + 4321);
}

int qPart(size_t* arr, int l, int r) {
    if (l != r) {
        size_t temp = arr[(l + r) / 2];
        arr[(l + r) / 2] = arr[r];
        arr[r] = temp;
    }
    size_t pivot = arr[r];

    int i = l - 1;
    for (int j = l; j <= r; j++) {
        if (arr[j] <= pivot) {
            i++;
            size_t temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    return i;
}

size_t search(size_t* arr, int k, int l, int r) {
    int left = l, right = r;
    while (1) {
        int mid = qPart(arr, left, right);

        if (mid == k) {
            return arr[mid];
        } else if (k < mid) {
            right = mid - 1;
        }  else {
            left = mid + 1;
        }
    }
}

// int comparator(const void *i, const void *j) {
//     return *((int*) i) - *((int*) j);
// }

int main() {
    int n = 0, k = 0;
    size_t a1 = 0, a2 = 2;
    INPUT_CHECK(scanf("%d %d %lu", &n, &k, &a1), 3);

    if (n == 1) {
        printf("%lu", a1);
        return 0;
    }
    INPUT_CHECK(scanf("%lu", &a2), 1);
    if (n == 2) {
        if (k == 1) {
            printf("%lu", a1);
        } else {
            printf("%lu", a2);
        }

        return 0;
    }

    size_t* arr = (size_t*) calloc(n + 1, sizeof(size_t));
    ON_ERROR(!arr, "Unable to alloc", -1);

    arr[1] = a1;
    arr[2] = a2;
    for (int i = 3; i <= n; i++) {
        arr[i] = newValue(arr[i - 2], arr[i - 1]);
    }

    // printf("BEFORE:\n");
    // for (int i = 1; i <= n; i++) {
    //     printf("%d ", arr[i]);
    // }
    // printf("\n");

    size_t kVal = search(arr, k, 1, n);

    //qsort (arr, n + 1, sizeof(size_t), comparator);
    // printf("RESULT: %lu %lu\n", kVal, arr[k]);

    // printf("AFTER:\n");
    // for (int i = 1; i <= n; i++) {
    //     printf("%d ", arr[i]);
    // }
    // printf("\n");

    printf("%lu", kVal);
    free(arr);

    return 0;
}