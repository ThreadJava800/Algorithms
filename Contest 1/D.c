#include "stdio.h"
#include "malloc.h"
#include "string.h"
#include "stdlib.h"

// Getting ready for review

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


int MAX_COMMAND_LINE = 64;
int MAX_NUMBER_CNT = 2 * 100000 + 1;

char ENQ_COM[]  = "enqueue";
char DEQ_COM[]   = "dequeue";
char FRT_COM[]  = "front";
char SIZE_COM[]  = "size";
char CLEAR_COM[] = "clear";
char MIN_COM[]  = "min";

char ERROR_MSG[] = "error";
char OK_MSG[]    = "ok";


typedef struct {
    int size;

    int* values;
} Stack_t;

typedef struct {
    Stack_t* stack1;
    Stack_t* stack2;

    int* min1;
    int* min2;
} Queue_t;

void runMainCycle();
void parseCommands(int* isRunning, char* command, Queue_t* queue);

Stack_t* stkInit();
void stkPush(Stack_t* stack, int value);
void stkResize(Stack_t* stack);
int stkPop(Stack_t* stack, int* success);
int stkBack(Stack_t* stack, int* success);
int stkSize(Stack_t* stack);
void stkClear(Stack_t* stack);
void stkDtor(Stack_t* stack);

Queue_t* queueCtr();
void _queueEnqSupport(Stack_t* values, int* mins, int value);
void queueEnq(Queue_t* queue, int value);
int queueDeq(Queue_t* queue, int* success);
int queueFront(Queue_t* queue, int* success);
int queueSize(Queue_t* queue);
void queueClear(Queue_t* queue);
int queueMin(Queue_t* queue, int* success);
void queueDtor(Queue_t* queue);


Stack_t* stkInit() {
    Stack_t* stack  = (Stack_t*) calloc(1, sizeof(Stack_t));
    if (!stack) return NULL;

    stack->values   = (int*)     calloc(MAX_NUMBER_CNT, sizeof(int));
    if (!stack->values) return NULL;

    return stack;
}

void stkPush(Stack_t* stack, int value) {
    if (!stack) return;

    stack->values[stack->size] = value;
    stack->size++;
}

int stkPop(Stack_t* stack, int* success) {
    if (!stack || !success) return -1;

    if (stack->size <= 0) {
        *success = 0;
        return -1;
    }

    stack->size--;
    int value = stack->values[stack->size];

    return value;
}

int stkBack(Stack_t* stack, int* success) {
    if (!stack || !success) return -1;

    if (stack->size <= 0) {
        *success = 0;
        return -1;
    }

    int value = stack->values[stack->size - 1];

    return value;
}

int stkSize(Stack_t* stack) {
    if (!stack) return -1;

    return stack->size;
}

void stkClear(Stack_t* stack) {
    if (!stack) return;

    stack->size = 0;
}

void stkDtor(Stack_t* stack) {
    if (!stack) return;

    free(stack->values);
    free(stack);
}

Queue_t* queueCtr() {
    Queue_t* queue  = (Queue_t*) calloc(1, sizeof(Queue_t));

    Stack_t* stack1 = stkInit();
    Stack_t* stack2 = stkInit();
    if (!queue) return NULL;

    queue->stack1 = stack1;
    queue->stack2 = stack2;

    queue->min1   = (int*) calloc(MAX_NUMBER_CNT, sizeof(int));
    queue->min2   = (int*) calloc(MAX_NUMBER_CNT, sizeof(int));

    return queue;
}

void _queueEnqSupport(Stack_t* values, int* mins, int value) {
    ON_ERROR(!values || !mins, "Null ptr, cringe",);

    stkPush(values, value);

    int stackSize = values->size;

    if (stackSize == 1) {
        mins[0] = value;
    } else {
        if (mins[stackSize - 2] > value) {
            mins[stackSize - 1] = value;
        } else {
            mins[stackSize - 1] = mins[stackSize - 2];
        }
    }
}

void queueEnq(Queue_t* queue, int value) {
    ON_ERROR(!queue, "Null ptr, cringe",);

    _queueEnqSupport(queue->stack1, queue->min1, value);
}

int queueDeq(Queue_t* queue, int* success) {
    ON_ERROR(!queue || !success, "Null ptr, cringe", -1);

    *success = 1;

    if (!stkSize(queue->stack2)) {
        while(stkSize(queue->stack1)) {
            int value = stkPop(queue->stack1, success);
            _queueEnqSupport(queue->stack2, queue->min2, value);
        }
    }

    return stkPop(queue->stack2, success);
}

int queueFront(Queue_t* queue, int* success) {
    ON_ERROR(!queue, "Null ptr, cringe", -1);

    *success = 1;

    if (!stkSize(queue->stack2)) {
        while(stkSize(queue->stack1)) {
            int value = stkPop(queue->stack1, success);
            _queueEnqSupport(queue->stack2, queue->min2, value);
        }
    }

    return stkBack(queue->stack2, success);
}

int queueSize(Queue_t* queue) {
    ON_ERROR(!queue, "Null ptr, cringe", -1);

    return stkSize(queue->stack1) + stkSize(queue->stack2);
}

void queueClear(Queue_t* queue) {
    ON_ERROR(!queue, "Null ptr",);

    stkClear(queue->stack1);
    stkClear(queue->stack2);
}

int queueMin(Queue_t* queue, int* success) {
    ON_ERROR(!queue, "Null ptr", -1);

    *success = 1;

    int size1 = queue->stack1->size;
    int size2 = queue->stack2->size;

    if (size1 == 0) {
        if (size2 == 0) {
            *success = 0;
            return -1;
        }
        return queue->min2[size2 - 1];
    } else if (size2 == 0) {
        return queue->min1[size1 - 1];
    } else if (queue->min1[size1 - 1] < queue->min2[size2 - 1]) {
        return queue->min1[size1 - 1];
    }

    return queue->min2[size2 - 1];
}

void queueDtor(Queue_t* queue) {
    ON_ERROR(!queue, "Null ptr, cringe.",);

    stkDtor(queue->stack1);
    stkDtor(queue->stack2);

    free(queue->min1);
    free(queue->min2);
    free(queue);
}


void runMainCycle() {
    int isRunning = 1;

    Queue_t* queue = queueCtr();
    if (!queue) return;

    int n = 0;
    INPUT_CHECK(scanf("%d", &n), 1);

    for(int i = 0; i < n; i++) {
        char* command = (char*) calloc(MAX_COMMAND_LINE, sizeof(char));
        if (!command) return;

        int res = scanf("%s", command);
        if (!res) return;

        parseCommands(&isRunning, command, queue);
        free(command);
    }

    queueDtor(queue);
}

void parseCommands(int* isRunning, char* command, Queue_t* queue) {
    if(!isRunning || !command || !queue) return;

    if (!strcmp(command, ENQ_COM)) {
        int value = 0;
        int res = scanf("%d", &value);
        if (!res) return;

        queueEnq(queue, value);
        fprintf(stdout, "%s\n", OK_MSG);

        return;
    }
    if (!strcmp(command, DEQ_COM)) {
        int success = 1;
        int value = queueDeq(queue, &success);

        if (success) fprintf(stdout, "%d\n", value);
        else fprintf(stdout, "%s\n", ERROR_MSG);

        return;
    }
    if (!strcmp(command, FRT_COM)) {
        int success = 1;
        int value = queueFront(queue, &success);

        if (success) fprintf(stdout, "%d\n", value);
        else fprintf(stdout, "%s\n", ERROR_MSG);

        return;
    }
    if (!strcmp(command, SIZE_COM)) {
        fprintf(stdout, "%d\n", queueSize(queue));

        return;
    }
    if (!strcmp(command, CLEAR_COM)) {
        queueClear(queue);
        fprintf(stdout, "%s\n", OK_MSG);

        return;
    }
    if (!strcmp(command, MIN_COM)) {
        int success = 1;
        int value = queueMin(queue, &success);

        if (success) fprintf(stdout, "%d\n", value);
        else fprintf(stdout, "%s\n", ERROR_MSG);

        return;
    }
}

int main() {
    runMainCycle();

    return 0;
}