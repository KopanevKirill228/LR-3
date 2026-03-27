#include <iostream>
#include <iomanip>
#include "SparseMatrix.h"
#include "RectangularMatrix.h"

using namespace std;

// Вспомогательная функция вывода матрицы
template <typename T>
void printMatrix(const char* title, IMatrix<T>* m) {
    cout << "\n" << title << ":\n";
    for (int i = 0; i < m->Rows(); i++) {
        for (int j = 0; j < m->Cols(); j++)
            cout << setw(6) << m->Get(i, j) << " ";
        cout << "\n";
    }
}

// Тест базовых операций
void testBasic() {
    cout << "=== Базовые операции ===\n";

    SparseMatrix<int> m(4, 4);
    m.Set(0, 0, 5);
    m.Set(1, 2, 3);
    m.Set(3, 3, 7);

    cout << "NonZeroCount: " << m.NonZeroCount() << " (ожидается 3)\n";
    cout << "Get(0,0): " << m.Get(0, 0) << " (ожидается 5)\n";
    cout << "Get(2,2): " << m.Get(2, 2) << " (ожидается 0)\n";

    // Set нуля должен удалить элемент
    m.Set(1, 2, 0);
    cout << "После Set(1,2,0) NonZeroCount: " << m.NonZeroCount() << " (ожидается 2)\n";
}

// Тест арифметических операций
void testArithmetic() {
    cout << "\n=== Арифметические операции ===\n";

    SparseMatrix<int> m(3, 3);
    m.Set(0, 0, 1);
    m.Set(1, 1, 2);
    m.Set(2, 2, 3);

    printMatrix("Исходная матрица", &m);

    // Умножение на скаляр
    IMatrix<int>* scaled = m.MulScalar(2);
    printMatrix("MulScalar(2)", scaled);
    delete scaled;

    // Транспонирование
    IMatrix<int>* transposed = m.Transpose();
    printMatrix("Transpose", transposed);
    delete transposed;
}

// Тест элементарных преобразований
void testRowOperations() {
    cout << "\n=== Преобразования строк ===\n";

    SparseMatrix<int> m(3, 3);
    m.Set(0, 0, 1); m.Set(0, 1, 2);
    m.Set(1, 0, 3); m.Set(1, 1, 4);
    m.Set(2, 2, 9);

    printMatrix("До SwapRows(0,2)", &m);
    m.SwapRows(0, 2);
    printMatrix("После SwapRows(0,2)", &m);
}

// Тест исключений
void testExceptions() {
    cout << "\n=== Обработка исключений ===\n";

    SparseMatrix<int> m(3, 3);
    m.Set(0, 0, 5);

    try {
        m.Get(10, 0);
        cout << "ERROR: должно было выбросить исключение\n";
    }
    catch (const out_of_range& e) {
        cout << "OK: Get(10,0) -> " << e.what() << "\n";
    }

    try {
        m.Set(-1, 0, 5);
        cout << "ERROR: должно было выбросить исключение\n";
    }
    catch (const out_of_range& e) {
        cout << "OK: Set(-1,0,5) -> " << e.what() << "\n";
    }
}

// Тест нормы матрицы
void testNorm() {
    cout << "\n=== Норма матрицы ===\n";

    SparseMatrix<double> m(3, 3);
    m.Set(0, 0, 1.0);
    m.Set(1, 1, 2.0);
    m.Set(2, 2, 3.0);

    double norm = m.Norm();
    cout << "Norm: " << fixed << setprecision(4) << norm;
    cout << " (ожидается ~3.7417)\n";
}

int main() {
    setlocale(LC_ALL, "Russian");

    cout << "========================================\n";
    cout << "Тестирование SparseMatrix\n";
    cout << "========================================\n";

    testBasic();
    testArithmetic();
    testRowOperations();
    testExceptions();
    testNorm();

    cout << "\n========================================\n";
    cout << "Все тесты завершены\n";
    cout << "========================================\n";

    return 0;
}