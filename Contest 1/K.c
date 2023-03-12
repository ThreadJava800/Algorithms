#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

// Quick Sort

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


int* readArray(int* size);
void mqsort(int* arr, int l, int r);
void printArray(int* arr, int size);


int* readArray(int* size) {
    ON_ERROR(!size, "Nullptr... cringe", NULL);

    int n = 0;
    INPUT_CHECK(scanf("%d", &n), 1);
    *size = n;

    int* array = (int*) calloc(n, sizeof(int));
    ON_ERROR(!array, "Unable to allocate memory!", NULL);

    for (int i = 0; i < n; i++) {
        INPUT_CHECK(scanf("%d", &(array[i])), 1);
    }

    return array;
}

void mqsort(int* arr, int l, int r) {
    ON_ERROR(!arr, "Nullptr... cringe",);
    ON_ERROR(l < 0 || r < 0 || l > r, "Incorrect borders!",);

    int i = l, j = r;
    int pivot = arr[(i + j) / 2];

    while (i <= j) {

        while (arr[i] < pivot) {
            i++;
        }
        while (arr[j] > pivot) {
            j--;
        }

        if (i <= j) {
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;

            i++;
            j--;
        }
    }

    if (l < j) mqsort(arr, l, j);
    if (i < r) mqsort(arr, i, r);
}

void printArray(int* arr, int size) {
    ON_ERROR(!arr, "Nullptr... cringe",);

    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
}


int main() {
    int size = 0;
    int* arr = readArray(&size);
    mqsort(arr, 0, size - 1);    
    printArray(arr, size);

    return 0;
}