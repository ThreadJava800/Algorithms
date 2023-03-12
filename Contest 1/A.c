#include "stdio.h"
#include "malloc.h"
#include "string.h"

// Getting ready for review

int MAX_COMMAND_LINE = 6;
int MAX_NUMBER_CNT = 10001;

char PUSH_COM[]  = "push";
char POP_COM[]   = "pop";
char BACK_COM[]  = "back";
char SIZE_COM[]  = "size";
char CLEAR_COM[] = "clear";
char EXIT_COM[]  = "exit";

char ERROR_MSG[] = "error";
char OK_MSG[]    = "ok";
char BYE_MSG[]    = "bye";


typedef struct {
    int size;

    int* values;
} Stack_t;

void runMainCycle();
void parseCommands(int* isRunning, char* command, Stack_t* stk);

Stack_t* stkInit();
void stkPush(Stack_t* stack, int value);
void stkResize(Stack_t* stack);
int stkPop(Stack_t* stack, int* success);
int stkBack(Stack_t* stack, int* success);
int stkSize(Stack_t* stack);
void stkClear(Stack_t* stack);
void stkDtor(Stack_t* stack);

void runMainCycle() {
    int isRunning = 1;

    Stack_t* stack = stkInit();
    if (!stack) return;

    while(isRunning) {
        char* command = (char*) calloc(MAX_COMMAND_LINE, sizeof(char));
        if (!command) return;

        int res = scanf("%s", command);
        if (!res) return;

        parseCommands(&isRunning, command, stack);
        free(command);
    }

    stkDtor(stack);
}

void parseCommands(int* isRunning, char* command, Stack_t* stk) {
    if(!isRunning || !command || !stk) return;

    if (!strcmp(command, PUSH_COM)) {
        int value = 0;
        int res = scanf("%d", &value);
        if (!res) return;

        stkPush(stk, value);
        fprintf(stdout, "%s\n", OK_MSG);

        return;
    }
    if (!strcmp(command, POP_COM)) {
        int success = 1;
        int value = stkPop(stk, &success);

        if (success) fprintf(stdout, "%d\n", value);
        else fprintf(stdout, "%s\n", ERROR_MSG);

        return;
    }
    if (!strcmp(command, BACK_COM)) {
        int success = 1;
        int value = stkBack(stk, &success);

        if (success) fprintf(stdout, "%d\n", value);
        else fprintf(stdout, "%s\n", ERROR_MSG);

        return;
    }
    if (!strcmp(command, SIZE_COM)) {
        fprintf(stdout, "%d\n", stkSize(stk));

        return;
    }
    if (!strcmp(command, CLEAR_COM)) {
        stkClear(stk);
        fprintf(stdout, "%s\n", OK_MSG);

        return;
    }
    if (!strcmp(command, EXIT_COM)) {
        *isRunning = 0;
        fprintf(stdout, "%s\n", BYE_MSG);
        return;
    }
}

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

int main() {
    runMainCycle();

    return 0;
}