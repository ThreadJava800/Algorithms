#include "malloc.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

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


size_t MAX_COMMAND_LINE = 64;
size_t MAX_NUMBER_CNT = 2 * 100000 + 1;

enum errors {
    NULLPTR             = -1,
    UNABLE_TO_ALLOC_MEM = -2,
    INCORRECT_INPUT     = -3,
    INCORRECT_COMMAND   = -4,

    NO_ERROR            = 0,
};

enum commandNums {
    ENQ_COM,
    DEQ_COM,
    FRT_COM,
    SIZE_COM,
    CLEAR_COM,
    MIN_COM,

    ERROR_MSG,
    OK_MSG
};

const char* const COMMANDS[] = {
    [ENQ_COM]   = "enqueue",
    [DEQ_COM]   = "dequeue",
    [FRT_COM]   = "front",
    [SIZE_COM]  = "size",
    [CLEAR_COM] = "clear",
    [MIN_COM]   = "min",

    [ERROR_MSG] = "error",
    [OK_MSG]    = "ok"
};

typedef struct {
    size_t size;

    int* values;
} Stack_t;

typedef struct {
    Stack_t* stack1;
    Stack_t* stack2;

    int* min1;
    int* min2;
} Queue_t;

errors findSolution();
bool parseCommands(const char* command, Queue_t* queue);

Stack_t* stkInit();
void stkPush(Stack_t* stack, const int value);
void stkResize(Stack_t* stack);
int stkPop(Stack_t* stack, int* success);
int stkBack(Stack_t* stack, int* success);
size_t stkSize(Stack_t* stack);
void stkClear(Stack_t* stack);
void stkDtor(Stack_t* stack);

Queue_t* queueCtr();
void _queueEnqSupport(Stack_t* values, int* mins, const int value);
void queueEnq(Queue_t* queue, const int value);
int queueDeq(Queue_t* queue, int* success);
int queueFront(Queue_t* queue, int* success);
size_t queueSize(Queue_t* queue);
void queueClear(Queue_t* queue);
int queueMin(Queue_t* queue, int* success);
void queueDtor(Queue_t* queue);


Stack_t* stkInit() {
    Stack_t* stack  = (Stack_t*) calloc(1, sizeof(Stack_t));
    if (!stack) return NULL;

    stack->values   = (int*)     calloc(MAX_NUMBER_CNT, sizeof(int));
    if (!stack->values) {
        free(stack);
        return NULL;
    }

    return stack;
}

void stkPush(Stack_t* stack, int value) {
    if (!stack) return;

    stack->values[stack->size] = value;
    ++stack->size;
}

int stkPop(Stack_t* stack, int* success) {
    if (!stack || !success) return -1;

    if (stack->size <= 0) {
        *success = 0;
        return -1;
    }

    --stack->size;
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

size_t stkSize(Stack_t* stack) {
    if (!stack) return 0xDEAD;

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

    size_t stackSize = values->size;

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

size_t queueSize(Queue_t* queue) {
    ON_ERROR(!queue, "Null ptr, cringe", 0);

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

    size_t size1 = queue->stack1->size;
    size_t size2 = queue->stack2->size;

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


errors findSolution() {
    Queue_t* queue = queueCtr();
    if (!queue) return NULLPTR;

    int n = 0;
    INPUT_CHECK(scanf("%d", &n), 1);

    for(int i = 0; i < n; i++) {
        char* command = (char*) calloc(MAX_COMMAND_LINE + 1, sizeof(char));
        if (!command) return UNABLE_TO_ALLOC_MEM;

        int res = scanf("%64s", command);
        if (!res) return INCORRECT_INPUT;

        int success = parseCommands(command, queue);
        free(command);

        if (!success) return INCORRECT_COMMAND;
    }

    queueDtor(queue);

    return NO_ERROR;
}

bool parseCommands(const char* command, Queue_t* queue) {
    if(!command || !queue) return false;

    int success = true;

    if (!strcmp(command, COMMANDS[ENQ_COM])) {
        int value = 0;
        int res = scanf("%d", &value);
        if (!res) return false;

        queueEnq(queue, value);
        fprintf(stdout, "%s\n", COMMANDS[OK_MSG]);

        return true;
    }
    if (!strcmp(command, COMMANDS[DEQ_COM])) {
        int value = queueDeq(queue, &success);

        if (success) fprintf(stdout, "%d\n", value);
        else fprintf(stdout, "%s\n", COMMANDS[ERROR_MSG]);

        return true;
    }
    if (!strcmp(command, COMMANDS[FRT_COM])) {
        int value = queueFront(queue, &success);

        if (success) fprintf(stdout, "%d\n", value);
        else fprintf(stdout, "%s\n", COMMANDS[ERROR_MSG]);

        return true;
    }
    if (!strcmp(command, COMMANDS[SIZE_COM])) {
        fprintf(stdout, "%lu\n", queueSize(queue));

        return true;
    }
    if (!strcmp(command, COMMANDS[CLEAR_COM])) {
        queueClear(queue);
        fprintf(stdout, "%s\n", COMMANDS[OK_MSG]);

        return true;
    }
    if (!strcmp(command, COMMANDS[MIN_COM])) {
        int value = queueMin(queue, &success);

        if (success) fprintf(stdout, "%d\n", value);
        else fprintf(stdout, "%s\n", COMMANDS[ERROR_MSG]);

        return true;
    }

    return false;
}

int main() {
    findSolution();

    return 0;
}