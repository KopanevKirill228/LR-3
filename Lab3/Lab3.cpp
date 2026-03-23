// main.cpp
#include <iostream>
#include "RectangularMatrix.h"

int main() {
    // 3x3 матрица из нулей
    RectangularMatrix<int> m(3, 3);

    // заполняем вручную
    m.Set(0, 0, 1); m.Set(0, 1, 2); m.Set(0, 2, 3);
    m.Set(1, 0, 4); m.Set(1, 1, 5); m.Set(1, 2, 6);
    m.Set(2, 0, 7); m.Set(2, 1, 8); m.Set(2, 2, 9);

    // печатаем
    std::cout << "Matrix:" << std::endl;
    for (int i = 0; i < m.Rows(); i++) {
        for (int j = 0; j < m.Cols(); j++)
            std::cout << m.Get(i, j) << " ";
        std::cout << "\n";
    }

    // Add
    auto* sum = m.Add(m);
    std::cout << "\nMatrix + Matrix:" << std::endl;
    for (int i = 0; i < sum->Rows(); i++) {
        for (int j = 0; j < sum->Cols(); j++)
            std::cout << sum->Get(i, j) << " ";
        std::cout << "\n";
    }
    delete sum;

    // MulScalar
    auto* scaled = m.MulScalar(2);
    std::cout << "\nMatrix * 2:" << std::endl;
    for (int i = 0; i < scaled->Rows(); i++) {
        for (int j = 0; j < scaled->Cols(); j++)
            std::cout << scaled->Get(i, j) << " ";
        std::cout << "\n";
    }
    delete scaled;

    // Norm
    std::cout << "\nNorm: " << m.Norm() << std::endl;

    // SwapRows
    m.SwapRows(0, 2);
    std::cout << "\nAfter SwapRows(0,2):" << std::endl;
    for (int i = 0; i < m.Rows(); i++) {
        for (int j = 0; j < m.Cols(); j++)
            std::cout << m.Get(i, j) << " ";
        std::cout << "\n";
    }

    std::cout << "\nOK" << std::endl;
    return 0;
}