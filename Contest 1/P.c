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


int CURRENT_INDEX = 0;
int CUR_IND_DELTA = 0;
int MAX_AMOUNT_OF_VALS = 1000000;    // (10 ** 6)
int MAX_COMMAND_LINE   = 30;

const char INSERT_COM[]  = "insert";
const char GETMIN_COM[]  = "getMin";
const char EXTRCT_COM[]  = "extractMin";
const char DECREA_COM[]  = "decreaseKey";

typedef struct {
    int size;
    long int* arr;

    long int* valIndArr;
    long int* indValArr;
} KeyBinHeap_t;

void runMainCycle();
void parseCommands(int* isRunning, char* command, KeyBinHeap_t* heap);

KeyBinHeap_t* heapCtor();
void heapAdd(KeyBinHeap_t* heap, long int value);
long int heapDel(KeyBinHeap_t* heap, int index);
void extractMin(KeyBinHeap_t* heap);
void decreaseKey(KeyBinHeap_t* heap, int key, long int delta);
void heapSiftDown(KeyBinHeap_t* heap, int index);
void heapSiftUp(KeyBinHeap_t* heap, int index);
void heapDtor(KeyBinHeap_t* heap);

void runMainCycle() {
    int isRunning = 1;

    KeyBinHeap_t* heap = heapCtor();
    if (!heap) return;

    int q = 0;
    INPUT_CHECK(scanf("%d", &q), 1);



    for(int i = 0; i < q; i++) {
        char* command = (char*) calloc(MAX_COMMAND_LINE, sizeof(char));
        if (!command) return;

        int res = scanf("%s", command);
        if (!res) return;

        parseCommands(&isRunning, command, heap);
        free(command);
    }

    heapDtor(heap);
}

void parseCommands(int* isRunning, char* command, KeyBinHeap_t* heap) {
    if(!isRunning || !command || !heap) return;

    CURRENT_INDEX++;

    if (!strcmp(command, INSERT_COM)) {
        long int x = 0;
        INPUT_CHECK(scanf("%ld", &x), 1);

        heapAdd(heap, x);

        return;
    }
    if (!strcmp(command, GETMIN_COM)) {
        printf("%ld\n", heap->arr[1]);

        return;
    }
    if (!strcmp(command, EXTRCT_COM)) {
        extractMin(heap);

        return;
    }
    if (!strcmp(command, DECREA_COM)) {
        long int key = 0, delta = 0;
        INPUT_CHECK(scanf("%ld %ld", &key, &delta), 2);

        decreaseKey(heap, key, delta);

        return;
    }
}

KeyBinHeap_t* heapCtor() {
    long int* vals    = (long int*) calloc(MAX_AMOUNT_OF_VALS + 1, sizeof(long int));
    long int* indArr  = (long int*) calloc(MAX_AMOUNT_OF_VALS + 1, sizeof(long int));
    long int* arrInd  = (long int*) calloc(MAX_AMOUNT_OF_VALS + 1, sizeof(long int));

    KeyBinHeap_t* heap = (KeyBinHeap_t*) calloc(1, sizeof(KeyBinHeap_t));
    ON_ERROR(!heap || !vals || !heap, "Nullptr", NULL);

    heap->arr       = vals;
    heap->size      = 1;

    heap->valIndArr = indArr;
    heap->indValArr = arrInd;

    return heap;
}

void heapAdd(KeyBinHeap_t* heap, long int value) {
    ON_ERROR(!heap, "Null ptr, Noob", );

    heap->arr[(heap->size)++]    = value;

    heap->indValArr[CURRENT_INDEX]  = heap->size - 1; 
    heap->valIndArr[heap->size - 1] = CURRENT_INDEX;

    heapSiftUp(heap, heap->size - 1);
}

void extractMin(KeyBinHeap_t* heap) {
    ON_ERROR(!heap, "Null ptr, Noob",);

    heap->arr[1] = heap->arr[--(heap->size)];
    
    heap->indValArr[heap->valIndArr[1]] = heap->indValArr[heap->valIndArr[heap->size]];
    heap->valIndArr[1] = heap->valIndArr[heap->size];

    heapSiftDown(heap, 1);
}

void decreaseKey(KeyBinHeap_t* heap, int key, long int delta) {
    ON_ERROR(!heap, "Null ptr, Noob",);

    heap->arr[heap->indValArr[key]] -= delta;
    heapSiftUp(heap, heap->indValArr[key]);
}

void heapSiftDown(KeyBinHeap_t* heap, int index) {
    ON_ERROR(!heap, "Null ptr, Noob", );

    while (index * 2 + 1 <= heap->size) {
        int leftSon  = index * 2;
        int rightSon = index * 2 + 1;

        if (heap->arr[leftSon]  > heap->arr[index] &&
            heap->arr[rightSon] > heap->arr[index]) {
                break;
        }

        if (heap->arr[leftSon] < heap->arr[rightSon]) {
            // swapping indexes
            long int indTemp = heap->valIndArr[leftSon];        // add position of leftson
            heap->valIndArr[leftSon] = heap->valIndArr[index];
            heap->valIndArr[index]  = indTemp;

            long int arrTemp = heap->indValArr[indTemp];        // real index by add position
            heap->indValArr[indTemp] = heap->indValArr[heap->valIndArr[leftSon]];
            heap->indValArr[heap->valIndArr[leftSon]]  = arrTemp;

            // swapping values
            long int temp = heap->arr[leftSon];
            heap->arr[leftSon] = heap->arr[index];
            heap->arr[index] = temp;

            index = leftSon;
        } else {
            // swapping indexes
            long int indTemp = heap->valIndArr[rightSon];        // add position of leftson
            heap->valIndArr[rightSon] = heap->valIndArr[index];
            heap->valIndArr[index]  = indTemp;

            long int arrTemp = heap->indValArr[indTemp];        // real index by add position
            heap->indValArr[indTemp] = heap->indValArr[heap->valIndArr[rightSon]];
            heap->indValArr[heap->valIndArr[rightSon]]  = arrTemp;


            // swapping values
            long int temp = heap->arr[rightSon];
            heap->arr[rightSon] = heap->arr[index];
            heap->arr[index] = temp;

            index = rightSon;
        }
    }
}

void heapSiftUp(KeyBinHeap_t* heap, int index) {
    ON_ERROR(!heap, "Null ptr, Noob", );

    long int* vals = heap->arr;
    while (vals[index] < vals[index / 2] && index != 1) {
        long int temp = vals[index];
        vals[index] = vals[index / 2];
        vals[index / 2] = temp;

        // swapping indexes
        long int indTemp = heap->valIndArr[index];        // add position of leftson
        heap->valIndArr[index] = heap->valIndArr[index / 2];
        heap->valIndArr[index / 2]  = indTemp;

        long int arrTemp = heap->indValArr[indTemp];        // real index by add position
        heap->indValArr[indTemp] = heap->indValArr[heap->valIndArr[index / 2]];
        heap->indValArr[heap->valIndArr[index / 2]]  = arrTemp;

        index /= 2;
    }
}

void heapDtor(KeyBinHeap_t* heap) {
    ON_ERROR(!heap, "Null ptr, Noob", );

    // free(heap->arr);
    // free(heap);
}

int main () {
    runMainCycle();

    return 0;
}