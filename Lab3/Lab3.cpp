#include <iostream>
#include "SquareMatrix.h"
#include "DiagonalMatrix.h"
#include "MatrixOperators.h"

int main() {
    double** data1 = new double* [3];
    double** data2 = new double* [3];
    for (int i = 0; i < 3; i++) {
        data1[i] = new double[3];
        data2[i] = new double[3];
        for (int j = 0; j < 3; j++) {
            data1[i][j] = (i == j) ? 2.0 : 1.0;
            data2[i][j] = (i == j) ? 3.0 : 0.0;
        }
    }
    SquareMatrix<double> m1(3, data1);
    SquareMatrix<double> m2(3, data2);
    for (int i = 0; i < 3; i++) { delete[] data1[i]; delete[] data2[i]; }
    delete[] data1; delete[] data2;

    // operator<<
    std::cout << "=== m1 ===" << std::endl;
    std::cout << m1;
    std::cout << "=== m2 ===" << std::endl;
    std::cout << m2;

    // operator+
    auto* sum = m1 + m2;
    std::cout << "=== m1 + m2 ===" << std::endl;
    std::cout << *sum;
    delete sum;

    // operator* scalar правый
    auto* scaled = m1 * 2.0;
    std::cout << "=== m1 * 2 ===" << std::endl;
    std::cout << *scaled;
    delete scaled;

    // operator* scalar левый
    auto* scaled2 = 3.0 * m1;
    std::cout << "=== 3 * m1 ===" << std::endl;
    std::cout << *scaled2;
    delete scaled2;

    // operator* матрицы
    auto* product = m1 * m2;
    std::cout << "=== m1 * m2 ===" << std::endl;
    std::cout << *product;
    delete product;

    // operator== / !=
    SquareMatrix<double> m3(m1);
    std::cout << "m1 == m1: " << (m1 == m3 ? "true" : "false") << std::endl;
    std::cout << "m1 == m2: " << (m1 == m2 ? "true" : "false") << std::endl;
    std::cout << "m1 != m2: " << (m1 != m2 ? "true" : "false") << std::endl;

    // Всё то же самое для DiagonalMatrix — операторы работают через IMatrix
    double diag1[] = { 1.0, 2.0, 3.0 };
    double diag2[] = { 4.0, 5.0, 6.0 };
    DiagonalMatrix<double> d1(3, diag1);
    DiagonalMatrix<double> d2(3, diag2);

    std::cout << "\n=== d1 ===" << std::endl;
    std::cout << d1;

    auto* dsum = d1 + d2;
    std::cout << "=== d1 + d2 ===" << std::endl;
    std::cout << *dsum;
    delete dsum;

    auto* dscaled = d1 * 3.0;
    std::cout << "=== d1 * 3 ===" << std::endl;
    std::cout << *dscaled;
    delete dscaled;

    auto* dproduct = d1 * d2;
    std::cout << "=== d1 * d2 ===" << std::endl;
    std::cout << *dproduct;
    delete dproduct;

    std::cout << "d1 == d2: " << (d1 == d2 ? "true" : "false") << std::endl;
    std::cout << "d1 == d1: " << (d1 == d1 ? "true" : "false") << std::endl;

    return 0;
}