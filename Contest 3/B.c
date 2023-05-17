#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>

typedef short (*CompareFunc_t)(int val1, int val2);
typedef int (*HashFunc_t)(int value);

const int DEFAULT_ARR_SIZE = 990001;

//=========CONTEST STAFF=============
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
//===========================================

//================LIST STAFF=================

struct ListElement_t {
    int value;

    struct ListElement_t *previous;
    struct ListElement_t *next;
};

struct List_t {
    struct ListElement_t *root;
    size_t size;
};

void                  listCtor    (struct List_t *list);
struct ListElement_t *newNode     (int value, struct ListElement_t *prev);
void                  listPushBack(struct List_t *list, int value);
short                 listFind    (struct List_t *list, int element, CompareFunc_t comparator);
void                  listDelete  (struct List_t *list, int value);
void                  listDtor    (struct List_t *list);
void listDump(struct List_t *list);
//=======================HASH MAP=======================
struct HashMap_t {
    struct List_t *listArr;
    int  listCnt;

    HashFunc_t hashFunc;
};

struct HashMap_t  *hashMapNew   (HashFunc_t hashFunc);
void               hashMapCtor  (struct HashMap_t *hashMap, HashFunc_t hashFunc);
void               hashMapDelete(struct HashMap_t *hashMap);
void               hashMapDtor  (struct HashMap_t *hashMap);

short              comparator(int val1, int val2);
void               hashMapInsert(struct HashMap_t *hashMap, int key);
void               hashMapRemove(struct HashMap_t *hashMap, int key);
short              hashMapSearch(struct HashMap_t *hashMap, int key);

int rotlHash (int value);
//======================================

void listCtor (struct List_t *list) {
    ON_ERROR(!list, "Nullptr", );

    list->size = 0;
    list->root = NULL;
}

struct ListElement_t *newNode(int value, struct ListElement_t *prev) {
    struct ListElement_t *node = (struct ListElement_t *) calloc(1, sizeof(struct ListElement_t));
    ON_ERROR(!node, "Nullptr", NULL);

    node->previous = prev;
    node->value    = value;

    return node;
}

void listPushBack(struct List_t *list, int value) {
    ON_ERROR(!list, "Nullptr", );

    if (!list->root) {
        list->root = newNode(value, NULL);
        return;
    }

    struct ListElement_t *curNode = list->root;
    while (curNode->next) curNode = curNode->next;
    
    curNode->next = newNode(value, curNode);
}

short listFind(struct List_t *list, int element, CompareFunc_t comparator) {
    ON_ERROR(!list, "Nullptr", 0);

    struct ListElement_t *curNode = list->root;
    while (curNode)
    {
        if (comparator(curNode->value, element)) return 1;
        curNode = curNode->next;
    }
    
    return 0;
}
void listDelete(struct List_t *list, int value) {
    ON_ERROR(!list, "Nullptr", );

    struct ListElement_t *curNode = list->root;
    while (curNode)
    {
        if (value == curNode->value) {
            if (!curNode->previous) {
                list->root = curNode->next;
                if (curNode->next) curNode->next->previous = NULL;

                // free(saveNode);
                curNode = list->root;

            } else if (!curNode->next) {
                struct ListElement_t *nextNode = curNode->previous;
                curNode->previous->next = NULL;

                curNode = nextNode;
                // free(curNode);
            } else {
                struct ListElement_t *nextNode = curNode->previous;
                curNode->previous->next = curNode->next;
                curNode->next->previous = curNode->previous;

                curNode = nextNode;

                // free(curNode);
            }
        }

        if (curNode && curNode->next) curNode = curNode->next;
    }
}

void listDtor (struct List_t *list) {
    ON_ERROR(!list, "Nullptr", );

    while (list->root)
    {   
        struct ListElement_t *saveNode = list->root;
        list->root = list->root->next;

        free(saveNode);
    }
    
    free(list);
}

void listDump(struct List_t *list) {
    ON_ERROR(!list, "Nullptr", );

    struct ListElement_t *curNode = list->root;
    while (curNode)
    {
        fprintf(stderr, "%d ", curNode->value);
        curNode = curNode->next;
    }
    fprintf(stderr, "\n");
}

struct HashMap_t *hashMapNew(HashFunc_t hashFunc) {
    ON_ERROR(!hashFunc, "Nullptr", NULL);

    struct HashMap_t *hashMap = (struct HashMap_t*) calloc(1, sizeof(struct HashMap_t));
    ON_ERROR(!hashMap, "Unable to alloc memory", NULL);

    hashMapCtor(hashMap, hashFunc);

    return hashMap;
}

void hashMapCtor(struct HashMap_t *hashMap, HashFunc_t hashFunc) {
    ON_ERROR(!hashFunc || !hashMap, "Nullptr",);

    struct List_t *listArr = (struct List_t *) calloc(DEFAULT_ARR_SIZE, sizeof(struct List_t));
    ON_ERROR(!listArr, "Unable to alloc memory", );

    for (size_t i = 0; i < DEFAULT_ARR_SIZE; i++) {
        listCtor(&listArr[i]);
    }

    hashMap->listArr  = listArr;
    hashMap->hashFunc = hashFunc;
    hashMap->listCnt  = DEFAULT_ARR_SIZE;  
}

void hashMapDelete(struct HashMap_t *hashMap) {
    ON_ERROR(!hashMap, "Nullptr",);

    hashMapDtor(hashMap);
    free(hashMap);
}

void hashMapDtor(struct HashMap_t *hashMap) {
    ON_ERROR(!hashMap, "Nullptr",);
    ON_ERROR(!hashMap->listArr, "Nullptr",);

    for (int i = 0; i < hashMap->listCnt; i++) {
        struct List_t listToFree = hashMap->listArr[i];

        listDtor(&listToFree);
    }

    free(hashMap->listArr);
}

void hashMapInsert(struct HashMap_t *hashMap, int key) {
    ON_ERROR(!hashMap, "Nullptr",);

    int hashSum = hashMap->hashFunc(key) % hashMap->listCnt;

    listPushBack(&(hashMap->listArr[hashSum]), key);

    // listDump(&(hashMap->listArr[hashSum]));
}

void hashMapRemove(struct HashMap_t *hashMap, int key) {
    ON_ERROR(!hashMap, "Nullptr", );

    int hashSum = hashMap->hashFunc(key) % hashMap->listCnt;

    listDelete(&(hashMap->listArr[hashSum]), key);

    // listDump(&(hashMap->listArr[hashSum]));
}

short comparator(int val1, int val2) {
    return val1 == val2;
}

short hashMapSearch(struct HashMap_t *hashMap, int key) {
    ON_ERROR(!hashMap || !(hashMap->listArr), "Nullptr", -1);

    int  hashSum    = hashMap->hashFunc(key) % hashMap->listCnt;
    struct List_t  searchList = hashMap->listArr[hashSum];

    return listFind(&searchList, key, comparator);
}

int rotlHash  (int value) {
    return (value << 1) | (value >> 31);
}

int main() {
    int n = 0;
    INPUT_CHECK(scanf("%d", &n), 1);

    struct HashMap_t *hashMap = hashMapNew(rotlHash);

    for (int i = 0; i < n; i++) {
        char inpBuffer[2];
        int  value  = 0;

        scanf("%s %d", inpBuffer, &value);
        // fprintf(stderr, "%d %d\n", symbol, value);

        switch (inpBuffer[0])
        {
        case '+':
            hashMapInsert(hashMap, value);
            // visualGraph(hashMap->listArr, "Add");
            break;
        case '?': {
            short res = hashMapSearch(hashMap, value);

            if (!res) printf("NO\n");
            else     printf("YES\n");

            break;
        }
        case '-':
            hashMapRemove(hashMap, value);
            break;
        default:
            break;
        }
    }

    return 0;
}