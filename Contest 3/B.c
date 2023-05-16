#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>

struct Pair_t {
    int key;
    int value;
};

typedef struct Pair_t Elem_t;
typedef bool (*CompareFunc_t)(Elem_t *val1, Elem_t *val2);

const Elem_t POISON          = {};
const int RESIZE_COEFFICIENT = 2;
const int MAX_COMMAND_LENGTH = 2 << 8;

struct ListElement_t {
    Elem_t value;
    long   previous;
    long   next;
};

struct List_t {
    struct ListElement_t *values;
    long free;

    long size;
    long capacity;

    short linearized;
    short needLinear;
};

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

void listCtor(struct List_t *list, long listSize, short needLinear);
void fillElemList(struct ListElement_t *listElems, long capacity);
long _listInsertPhys(struct List_t *list, Elem_t value, long index);
long listInsert(struct List_t *list, Elem_t value, long index);
long listPushBack(struct List_t *list, Elem_t value);
long listPushFront(struct List_t *list, Elem_t value);
Elem_t _listRemovePhys(struct List_t *list, long index);
Elem_t listRemove(struct List_t *list, long index);
struct ListElement_t *listFind(struct List_t *list, Elem_t *searchElem, CompareFunc_t comparator);
[[nodiscard]] long logicToPhysics(struct List_t *list, long logicIndex);

inline __attribute__((always_inline)) Elem_t listGet(struct List_t *list, long index) {
    ON_ERROR(!list || index > list->capacity || list->size == 0, "Nullptr", POISON);

    long physIndex = logicToPhysics(list, index);
    return list->values[physIndex].value;
}

void listLinearize(struct List_t *list);
void listResize(struct List_t *list, long newCapacity);
void listRealloc(struct List_t *list, long newCapacity);
void poisonList(struct List_t *list, long newCapacity, long oldCapacity);
void listDtor(struct List_t *list);
long physicToLogic(struct List_t *list, long start, long phys);

void listCtor(struct List_t *list, long listSize, short needLinear) {\
    ON_ERROR(!list, "Nullptr", );

    list->values = (struct ListElement_t*) calloc((size_t) listSize + 1, sizeof(struct ListElement_t));
    ON_ERROR(!list->values, "Cannot alloc mem", );
    list->capacity = listSize;

    fillElemList(list->values, listSize);

    list->needLinear = needLinear;
    list->linearized = 1;
    list->free       = 1;
    list->size       = 0;
}

void fillElemList(struct ListElement_t *listElems, long capacity) {
    ON_ERROR(!listElems, "Nullptr", );

    listElems[0].value    = POISON;
    listElems[0].previous = 0;
    listElems[0].next     = 0;

    for (long i = 1; i < capacity + 1; i++) {
        listElems[i].value    = POISON;
        listElems[i].previous = -1;
        
        if (i != capacity - 1) {
            listElems[i].next =  i + 1;
        } else {
            listElems[i].next = 0;
        }
    }
}

long _listInsertPhys(struct List_t *list, Elem_t value, long index) {
    ON_ERROR(!list || index > list->capacity + 1, "Nullptr", -1);

    if (list->capacity - list->size <= 2) {
        long newCapacity = list->capacity * RESIZE_COEFFICIENT;
        listResize(list, newCapacity);
    }

    if (index != list->values[0].previous) {
        list->linearized = 0;
    }

    long nextFree =  list->values[list->free].next;
    long pushInd  = list->free;

    list->values[pushInd].value     = value;
    list->values[pushInd].next      = list->values[index].next;
    list->values[pushInd].previous  = index;

    list->values[list->values[index].next].previous = list->free;
    list->values[index]                   .next     = list->free;
 
    list->size++;
    list->free = nextFree;

    return pushInd;
}

long listInsert(struct List_t *list, Elem_t value, long index) {
    ON_ERROR(!list || index > list->capacity + 1, "Nullptr", -1);

    long physIndex = logicToPhysics(list, index);
    return _listInsertPhys(list, value, physIndex);
}

long listPushBack(struct List_t *list, Elem_t value) {
    ON_ERROR(!list, "Nullptr", -1);

    long lastInd = list->values[0].previous;
    return _listInsertPhys(list, value, lastInd);
}

long listPushFront(struct List_t *list, Elem_t value) {
    ON_ERROR(!list, "Nullptr", -1);

    return _listInsertPhys(list, value, 0);
}

Elem_t _listRemovePhys(struct List_t *list, long index) {
    ON_ERROR(!list || index > list->capacity + 1 || list->size == 0, "Nullptr", POISON);

    if (index != list->values[0].previous) {
        list->linearized = 0;
    }

    Elem_t returnValue = list->values[index].value;

    list->values[index].value = POISON;

    list->values[list->values[index].previous].next     = list->values[index].next;
    list->values[list->values[index].next]    .previous = list->values[index].previous;
    list->values[index]                       .next     = list->free;
    list->values[index]                       .next     = list->free;
    list->values[index]                       .previous = -1;

    list->size--;
    list->free = index;

    return returnValue;
}

Elem_t listRemove(struct List_t *list, long index) {
    ON_ERROR(!list || index > list->capacity + 1 || list->size == 0, "Nullptr", POISON);

    long physIndex = logicToPhysics(list, index);
    return _listRemovePhys(list, physIndex);
}

struct ListElement_t *listFind(struct List_t *list, Elem_t *searchElem, CompareFunc_t comparator) {
    ON_ERROR(!list || !comparator || !searchElem, "Nullptr", NULL);

    for (long i = 0; i < list->size; i++) {
        if (comparator(searchElem, &(list->values[i].value))) return &(list->values[i]);
    }

    return NULL;
}

[[nodiscard]] long logicToPhysics(struct List_t *list, long logicIndex) {
    ON_ERROR(!list || logicIndex > list->capacity, "Nullptr", -1);

    if (list->size == 0) return 0;

    long pos = list->values[0].next;
    if (logicIndex == 0) return pos;

    if (list->linearized) return logicIndex;

    for (long i = 0; i < logicIndex; i++) {
        if (list->values[pos].next == 0) {
            return pos;
        }
        pos =  list->values[pos].next;
    }

    return pos;
}

void listLinearize(struct List_t *list) {
    ON_ERROR(!list, "Nullptr", );

    if (list->linearized) return;

    struct ListElement_t *elements = (struct ListElement_t *) calloc((size_t) list->capacity + 1, sizeof(struct ListElement_t));
    ON_ERROR(!elements, "Nullptr", );

    fillElemList(elements, list->capacity);

    long oldIndex = list->values[0].next;
    for (long i = 0; i < list->size; i++) {
        elements[i + 1].value = list->values[oldIndex].value;

        if (i + 1 >= list->size) elements[i + 1].next  = 0;
        else                     elements[i + 1].next  = i + 2;

        if (i == 0) elements[i + 1].previous = 0;
        else        elements[i + 1].previous =  i;

        oldIndex =  list->values[oldIndex].next;
    }

    elements[0].next = 1;
    elements[0].previous = list->size;

    free(list->values);
    list->values     = elements;
    list->linearized = 1;
    list->free       = list->size + 1;
}

void listResize(struct List_t *list, long newCapacity) {
    ON_ERROR(!list, "Nullptr", );

    if (list->needLinear) {
        listLinearize(list);
    }

    if (newCapacity < list->capacity) {
        listRealloc(list, newCapacity);
        list->values[newCapacity - 1].next = 0;

        return;
    }

    long oldCapacity = list->capacity;
    listRealloc(list, newCapacity);
    poisonList (list, newCapacity, oldCapacity);
}

void listRealloc(struct List_t *list, long newCapacity) {
    ON_ERROR(!list || !list->values, "Nullptr", );

    list->values = (struct ListElement_t *) realloc(list->values, (size_t) newCapacity * sizeof(struct ListElement_t));
    ON_ERROR(!list->values, "Nullptr", );

    list->capacity = newCapacity;
}

void poisonList(struct List_t *list, long newCapacity, long oldCapacity) {
    ON_ERROR(!list || !list->values || newCapacity < oldCapacity, "Nullptr", );

    list->values[list->free].next =  oldCapacity;

    for (long i = oldCapacity; i < newCapacity; i++) {
        list->values[i].value    = POISON;
        list->values[i].previous = -1;

        if (i == newCapacity - 1) {
            list->values[i].next = 0;
        } else {
            list->values[i].next =  i + 1;
        }
    }
}

void listDtor(struct List_t *list) {
    ON_ERROR(!list, "Nullptr", );

    if (list->values) {
        free(list->values);
    }

    list->free       = 0xBEEF;
    list->size       = 0xBEEF;
    list->capacity   = 0xBEEF;
    list->linearized = 1;
}

long physicToLogic(struct List_t *list, long start, long phys) {
    ON_ERROR(!list, "Nullptr", -1);

    long logic = 0;
    long physInd = start;
    while (physInd != phys) {
        logic++;
        physInd = list->values[physInd].next;
    }

    return logic;
}

int main() {

    return 0;
}