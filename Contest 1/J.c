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


size_t invCount = 0;


int* readArray(int* size);
int* mergeSort(int* arr, int* buffer, int l, int r);
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

int* mergeSort(int* arr, int* buffer, int l, int r) {
    ON_ERROR(!arr, "Nullptr... cringe", NULL);
    ON_ERROR(l < 0 || r < 0 || l > r, "Incorrect borders!", NULL);

    if (l == r) {
        buffer[l] = arr[l];
        return buffer;
    }

    int middle = (l + r) / 2;
    int* lSide = mergeSort(arr, buffer, l, middle);
    int* rSide = mergeSort(arr, buffer, middle + 1, r);

    int* target = NULL;
    if (lSide == arr) {
        target = buffer;
    } else {
        target = arr;
    }

    int curR = middle + 1, curL = l;
    for (int i = l; i <= r; i++) {
        if (curL <= middle && curR <= r) {
            if (lSide[curL] <= rSide[curR]) {
                target[i] = lSide[curL++];
            } else {
                target[i] = rSide[curR++];
                invCount += (middle - curL + 1);
            }
        } else if (curL <= middle) {
            target[i] = lSide[curL++];
        } else {
            target[i] = rSide[curR++];
        }
    }

    return target;
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
    int* buffer = (int*) calloc(size, sizeof(int));

    int* sorted = mergeSort(arr, buffer, 0, size - 1);    
    // printArray(sorted, size);
    if (!sorted) return -1;

    printf("%lu\n", invCount);

    free(arr);
    free(buffer);

    return 0;
}