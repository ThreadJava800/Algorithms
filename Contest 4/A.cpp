#include <iostream>
#include <cmath>

size_t* mulMatrix(size_t* matrix1, size_t* matrix2);
size_t* matrixPow(size_t* matrix, size_t power);
size_t getFib(size_t n);

size_t* mulMatrix(size_t* matrix1, size_t* matrix2) { 
    if (!matrix1 || !matrix2) return nullptr;

    size_t* result = new size_t[2 * 2];

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            result[i * 2 + j] = 0;
        }
    }

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                result[i * 2 + j] = (result[i * 2 + j] + (matrix1[i * 2 + k] * matrix2[k * 2 + j]) % 1000003) % 1000003;
            }
        }
    }

    return result;
}

size_t* matrixPow(size_t* matrix, size_t power) {
    if (power == 1) return matrix;

    if (power % 2 == 1) {
        return mulMatrix(matrixPow(matrix, power - 1), matrix);
    }

    matrix = matrixPow(matrix, power / 2);
    return mulMatrix(matrix, matrix);
}

size_t getFib(size_t n) {
    if (n == 0 || n == 1) return n;

    size_t matrix[2 * 2] = {
        1, 1,
        1, 0
    };

    size_t* result = matrixPow(matrix, n - 1);
    return result[0];
}

int main() {
    size_t n = 0;
    std::cin >> n;

    std::cout << getFib(n - 1) % 1000003 << '\n';

    return 0;
}