#include <iostream>
#include <cmath>

#define ON_ERROR(expr, errStr, retVal) {                 \
    if (expr) {                                           \
        fprintf(stderr, "FATAL ERROR: %s\n", errStr);      \
        return retVal;                                      \
    }                                                        \
}                                                             \


size_t* mulMatrix(size_t* matrix1, size_t* matrix2, size_t matrixSize);
size_t* matrixPow(size_t* matrix,  size_t power,    size_t matrixSize);
size_t  getAmount(size_t n);

static const size_t MOD_NUMBER = 1000003;

size_t* mulMatrix(size_t* matrix1, size_t* matrix2, size_t matrixSize) { 
    ON_ERROR(!matrix1 || !matrix2, "Matrixes are nullptr!", nullptr);

    size_t* result = new size_t[matrixSize * matrixSize];
    ON_ERROR(!result, "Cannot allocate memory!", nullptr);

    for (size_t i = 0; i < matrixSize; i++) {
        for (size_t j = 0; j < matrixSize; j++) {
            result[i * matrixSize + j] = 0;
        }
    }

    for (size_t i = 0; i < matrixSize; i++) {
        for (size_t j = 0; j < matrixSize; j++) {
            for (size_t k = 0; k < matrixSize; k++) {
                result[i * matrixSize + j] = (result[i * matrixSize + j] + (matrix1[i * matrixSize + k] * matrix2[k * matrixSize + j]) % MOD_NUMBER) % MOD_NUMBER;
            }
        }
    }

    return result;
}

size_t* matrixPow(size_t* matrix, size_t power, size_t matrixSize) {
    ON_ERROR(!matrix, "Matrix is nullptr!", nullptr);

    if (power == 1) return matrix;

    if (power % 2 == 1) {
        return mulMatrix(matrixPow(matrix, power - 1, matrixSize), matrix, matrixSize);
    }

    matrix = matrixPow(matrix, power / 2, matrixSize);
    return mulMatrix(matrix, matrix, matrixSize);
}

size_t getAmount(size_t n) {
    if (n == 0 || n == 1) return n;

    size_t matrix[5 * 5] = {
        1, 1, 1, 1, 1,
        1, 0, 0, 0, 0,
        0, 1, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 0, 1, 0
    };

    size_t* result = matrixPow(matrix, n - 1, 5);
    ON_ERROR(!result, "Matrix is nullptr!", 0);

    size_t answer = result[0];

    delete[] result;

    return answer;
}

int main() {
    size_t n = 0;
    std::cin >> n;

    std::cout << getAmount(n) % MOD_NUMBER << '\n';

    return 0;
}