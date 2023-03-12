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

typedef struct {
    int size;
    int capacity;
    size_t* arr;
} Heap_t;

Heap_t* heapCtor(int n);
void heapAdd(Heap_t* heap, size_t value);
size_t heapDel(Heap_t* heap, int index);
int getMaxIndex(Heap_t* heap);
size_t extractMin(Heap_t* heap);
void heapSiftDown(Heap_t* heap, int index);
void heapSiftUp(Heap_t* heap, int index);
void heapDtor(Heap_t* heap);

size_t countNext(size_t a, size_t x, size_t y);

Heap_t* heapCtor(int n) {
    size_t* vals = (size_t*) calloc(n + 1, sizeof(size_t));
    Heap_t* heap = (Heap_t*) calloc(1, sizeof(Heap_t));
    ON_ERROR(!heap || !vals, "Nullptr", NULL);

    heap->arr       = vals;
    heap->size      = 1;
    heap->capacity  = n;

    return heap;
}

void heapAdd(Heap_t* heap, size_t value) {
    ON_ERROR(!heap, "Null ptr, Noob", );

    if (heap->size > heap->capacity) {
        if (value < heap->arr[1]) {
            heap->arr[1] = value;
            heapSiftDown(heap, 1);
        }

        return;
    }

    heap->arr[(heap->size)++] = value;
    heapSiftUp(heap, heap->size - 1);
}

size_t heapDel(Heap_t* heap, int index) {
    ON_ERROR(!heap, "Null ptr, Noob", -1);

    size_t val = heap->arr[index];
    heap->arr[index] = heap->arr[--(heap->size)];
    heapSiftDown(heap, index);

    return val;
}

int getMaxIndex(Heap_t* heap) {
    ON_ERROR(!heap, "Null ptr, Noob", -1);

    size_t maxi = 0;
    int index = 1;
    for (int i = heap->size / 2; i < heap->size; i++) {
        if (maxi <= heap->arr[i]) {
            maxi = heap->arr[i];
            index = i;
        }
    }

    return index;
}

size_t extractMin(Heap_t* heap) {
    ON_ERROR(!heap, "Null ptr, Noob", -1);

    return heapDel(heap, 1);
}

void heapSiftDown(Heap_t* heap, int index) {
    ON_ERROR(!heap, "Null ptr, Noob", );

    while (index * 2 + 1 <= heap->size) {
        int leftSon  = index * 2;
        int rightSon = index * 2 + 1;

        if (heap->arr[leftSon]  < heap->arr[index] &&
            heap->arr[rightSon] < heap->arr[index]) {
                break;
        }

        if (heap->arr[leftSon] > heap->arr[rightSon]) {
            size_t temp = heap->arr[leftSon];
            heap->arr[leftSon] = heap->arr[index];
            heap->arr[index] = temp;

            index = leftSon;
        } else {
            size_t temp = heap->arr[rightSon];
            heap->arr[rightSon] = heap->arr[index];
            heap->arr[index] = temp;

            index = rightSon;
        }
    }
}

void heapSiftUp(Heap_t* heap, int index) {
    ON_ERROR(!heap, "Null ptr, Noob", );

    size_t* vals = heap->arr;
    while (vals[index] > vals[index / 2] && index != 1) {
        size_t temp = vals[index];
        vals[index] = vals[index / 2];
        vals[index / 2] = temp;

        index /= 2;
    }
}

void heapDtor(Heap_t* heap) {
    ON_ERROR(!heap, "Null ptr, Noob", );

    free(heap->arr);
    free(heap);
}

size_t countNext(size_t a, size_t x, size_t y) {
    return (x * a + y) % (1073741824);  // 2 ^ 30
}

int main() {
    int n = 0, k = 0;
    INPUT_CHECK(scanf("%d %d", &n, &k), 2);

    Heap_t* heap = heapCtor(k);

    size_t a = 0, x = 0, y = 0;
    INPUT_CHECK(scanf("%lu %lu %lu", &a, &x, &y), 3);

    for (int i = 1; i <= n; i++) {
        size_t val = countNext(a, x, y);
        heapAdd(heap, val);
        a = val;
    }

    // for (int i = 1; i <= n; i++) {
    //     size_t val = 0;
    //     INPUT_CHECK(scanf("%lu", &val), 1);

    //     heapAdd(heap, val);
    // }

    // printf("\n");
    // for (int i = 1; i <= k; i++) {
    //     printf("%lu ", heap->arr[i]);
    // }
    // printf("\n");

    // for (int i = 1; i <= n; i++) {
    //     printf("%lld ", heap->arr[i]);
    // }
    // printf("\n");

    size_t* printArr = (size_t*) calloc(k, sizeof(size_t));

    for (int i = 0; i < k; i++) {
        printArr[i] = extractMin(heap);
    }

    for (int i = k - 1; i >= 0; i--) {
        printf("%lu ", printArr[i]);
    }

    heapDtor(heap);

    return 0;
}