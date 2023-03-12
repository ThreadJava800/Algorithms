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

size_t mask = 255;

size_t* readArray(int* size);
void strangeSort(size_t* arr, size_t* buffer, size_t* cntArr, int size);
void printArray(size_t* arr, int size);


size_t* readArray(int* size) {
    ON_ERROR(!size, "Nullptr... cringe", NULL);

    int n = 0;
    INPUT_CHECK(scanf("%d", &n), 1);
    *size = n;

    size_t* array = (size_t*) calloc(n, sizeof(size_t));
    ON_ERROR(!array, "Unable to allocate memory!", NULL);

    for (int i = 0; i < n; i++) {
        INPUT_CHECK(scanf("%lu", array + i), 1);
    }

    return array;
}

void strangeSort(size_t* arr, size_t* buffer, size_t* cntArr, int size) {
    ON_ERROR(!arr, "Nullptr... cringe",);

    for (int i = 0; i < 8; i++) {
       for (int j = 0;  j < 256; j++) {
            cntArr[j] = 0;
       }

       for (int j = 0; j < size; j++) {
            int val = ((arr[j] & mask) >> (8 * i));
            cntArr[val]++;
       }

       int count = 0;
       for (int j = 0; j < 256; j++) {
            size_t tmp = cntArr[j];
            cntArr[j] = count;
            count += tmp;
       }
       for (int j = 0; j < size; j++) {
            size_t val = ((arr[j] & mask) >> (8 * i));
            buffer[cntArr[val]] = arr[j];
            cntArr[val]++;
       }

       mask *= 256;
       for (int j = 0; j < size; j++) {
             arr[j] = buffer[j];
       }
    }
}

void printArray(size_t* arr, int size) {
    ON_ERROR(!arr, "Nullptr... cringe",);

    for (int i = 0; i < size; i++) {
        printf("%lu\n", arr[i]);
    }
}


int main() {
    int size = 0;
    size_t* arr = readArray(&size);
    size_t* buffer = (size_t*) calloc(size, sizeof(size_t));
    size_t* cntArr = (size_t*) calloc(256, sizeof(size_t));

    strangeSort(arr, buffer, cntArr, size);    
    printArray(arr, size);

    // free(arr);
    // free(buffer);
    // free(cntArr);

    return 0;
}