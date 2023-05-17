#include "malloc.h"
#include "stdio.h"
#include "string.h"

size_t MAX_COMMAND_LINE = 6;
size_t MAX_NUMBER_CNT = 10001;

#define true 1
#define false 0

enum errors {
    NULLPTR             = -1,
    UNABLE_TO_ALLOC_MEM = -2,
    INCORRECT_INPUT     = -3,

    NO_ERROR            = 0,
};

enum commandNums {
    PUSH_COM,
    POP_COM,
    BACK_COM,
    SIZE_COM,
    CLEAR_COM,
    EXIT_COM,

    ERROR_MSG,
    OK_MSG,
    BYE_MSG,
};

const char * const COMMANDS[] = {
    [PUSH_COM]  = "push",
    [POP_COM]   = "pop",
    [BACK_COM]  = "back",
    [SIZE_COM]  = "size",
    [CLEAR_COM] = "clear",
    [EXIT_COM]  = "exit",

    [ERROR_MSG] = "error",
    [OK_MSG]    = "ok",
    [BYE_MSG]   = "bye"
};


typedef struct {
    size_t size;

    int* values;
} Stack_t;

enum errors findSolution();
bool parseCommands(const char* command, Stack_t* stk);

Stack_t* stkInit();
void stkPush(Stack_t* stack, int value);
void stkResize(Stack_t* stack);
int stkPop(Stack_t* stack, bool* success);
int stkBack(Stack_t* stack, bool* success);
size_t stkSize(Stack_t* stack);
void stkClear(Stack_t* stack);
void stkDtor(Stack_t* stack);

enum errors findSolution() {
    bool isRunning = true;

    Stack_t* stack = stkInit();
    if (!stack) return NULLPTR;

    while(isRunning) {
        char* command = (char*) calloc(MAX_COMMAND_LINE + 1, sizeof(char));
        if (!command) return UNABLE_TO_ALLOC_MEM;

        int res = scanf("%6s", command);
        if (!res) return INCORRECT_INPUT;

        isRunning = parseCommands(command, stack);
        free(command);
    }

    stkDtor(stack);

    return NO_ERROR;
}

bool parseCommands(const char* command, Stack_t* stk) {
    if (!command || !stk) return false;

    bool success = false;

    if (!strcmp(command, COMMANDS[PUSH_COM])) {
        int value = 0;
        int res = scanf("%d", &value);
        if (!res) return false;

        stkPush(stk, value);
        fprintf(stdout, "%s\n", COMMANDS[OK_MSG]);

        return true;
    }
    if (!strcmp(command, COMMANDS[POP_COM])) {
        int value = stkPop(stk, &success);

        if (success) fprintf(stdout, "%d\n", value);
        else fprintf(stdout, "%s\n", COMMANDS[ERROR_MSG]);

        return true;
    }
    if (!strcmp(command, COMMANDS[BACK_COM])) {
        int value = stkBack(stk, &success);

        if (success) fprintf(stdout, "%d\n", value);
        else fprintf(stdout, "%s\n", COMMANDS[ERROR_MSG]);

        return true;
    }
    if (!strcmp(command, COMMANDS[SIZE_COM])) {
        fprintf(stdout, "%lu\n", stkSize(stk));

        return true;
    }
    if (!strcmp(command, COMMANDS[CLEAR_COM])) {
        stkClear(stk);
        fprintf(stdout, "%s\n", COMMANDS[OK_MSG]);

        return true;
    }
    if (!strcmp(command, COMMANDS[EXIT_COM])) {
        fprintf(stdout, "%s\n", COMMANDS[BYE_MSG]);
        return false;
    }

    return true;
}

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

int stkPop(Stack_t* stack, bool* success) {
    if (!stack || !success) return -1;

    if (stack->size <= 0) {
        *success = false;
        return -1;
    }

    --stack->size;
    int value = stack->values[stack->size];

    *success = true;
    return value;
}

int stkBack(Stack_t* stack, bool* success) {
    if (!stack || !success) return -1;

    if (stack->size <= 0) {
        *success = false;
        return -1;
    }

    int value = stack->values[stack->size - 1];

    *success = true;
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

int main() {
    findSolution();

    return 0;
}