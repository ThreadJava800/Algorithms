#include "stdio.h"
#include "stdlib.h"

#define INPUT_CHECK(readFunc, res) {        \
    int factRes = readFunc;                  \
    if (factRes != res) abort();              \
}                                              \

int main() {
    int amount = 0;
    INPUT_CHECK(scanf("%d", &amount), 1);
    
    int* array = (int*) calloc(amount, sizeof(int));
    for (int i = 0; i < amount; i++) {
        INPUT_CHECK(scanf("%d", array + i), 1);
    }

    INPUT_CHECK(scanf("%d", &amount), 1);

    for (int m = 0; m < amount; m++) {
        int i = 0, j = 0, t = 0;
        INPUT_CHECK(scanf("%d %d %d", &i, &j, &t), 3);
        j--;

        int isMet = 0;
        while (i <= j) {
            int pivot = i + (j - i) / 2;
            if (array[pivot] == t) {
                printf("YES\n");
                isMet = 1;
                break;
            }

            if (t < array[pivot]) {
                j = pivot - 1;
            }
            else {
                i = pivot + 1;
            }
        }

        if (!isMet) printf("NO\n");
    }

    return 0;
}