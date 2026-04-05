#pragma once
#include "SquareMatrix.h"
#include "RectangularMatrix.h"
#include "lib/ArraySequence.h"
#include <stdexcept>
#include <cmath>

template <class T>
struct LUResult {
    RectangularMatrix<T> L;
    RectangularMatrix<T> U;
    LUResult(int n) : L(n, n), U(n, n) {}
};

// Норма вектора
template <class T>
double VectorNorm(const MutableArraySequence<T>& v) {
    double sum = 0.0;
    for (int i = 0; i < v.GetLength(); i++)
        sum += (double)v.Get(i) * (double)v.Get(i);
    return std::sqrt(sum);
}

// Невязка: ||A*x - b||
template <class T>
double Residual(const IMatrix<T>& A,
    const MutableArraySequence<T>& x,
    const MutableArraySequence<T>& b) {
    int n = A.Rows();
    MutableArraySequence<T> diff;
    for (int i = 0; i < n; i++) {
        T sum = T();
        for (int j = 0; j < n; j++)
            sum = sum + A.Get(i, j) * x.Get(j);
        diff.Append(sum - b.Get(i));
    }
    return VectorNorm(diff);
}

// Прямая подстановка: L*y = b
template <class T>
MutableArraySequence<T> ForwardSubstitution(const IMatrix<T>& L,
    const MutableArraySequence<T>& b) {
    int n = L.Rows();
    MutableArraySequence<T> y;
    for (int i = 0; i < n; i++) y.Append(T());

    for (int i = 0; i < n; i++) {
        T sum = b.Get(i);
        for (int j = 0; j < i; j++)
            sum = sum - L.Get(i, j) * y.Get(j);
        y.Set(i, sum / L.Get(i, i));
    }
    return y;
}

// Обратная подстановка: U*x = y
template <class T>
MutableArraySequence<T> BackSubstitution(const IMatrix<T>& U,
    const MutableArraySequence<T>& y) {
    int n = U.Rows();
    MutableArraySequence<T> x;
    for (int i = 0; i < n; i++) x.Append(T());

    for (int i = n - 1; i >= 0; i--) {
        T sum = y.Get(i);
        for (int j = i + 1; j < n; j++)
            sum = sum - U.Get(i, j) * x.Get(j);
        x.Set(i, sum / U.Get(i, i));
    }
    return x;
}

// Метод Гаусса без выбора ведущего элемента
template <class T>
MutableArraySequence<T> GaussNopivot(const SquareMatrix<T>& A,
    const MutableArraySequence<T>& b) {
    int n = A.Rows();
    if (A.Cols() != n || b.GetLength() != n)
        throw std::invalid_argument("Invalid dimensions");

    RectangularMatrix<T> M(n, n + 1);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            M.Set(i, j, A.Get(i, j));
        M.Set(i, n, b.Get(i));
    }

    for (int col = 0; col < n; col++) {
        T pivot = M.Get(col, col);
        if (std::abs((double)pivot) < 1e-12)
            throw std::runtime_error("Zero pivot encountered");
        for (int row = col + 1; row < n; row++) {
            T factor = M.Get(row, col) / pivot;
            for (int j = col; j <= n; j++)
                M.Set(row, j, M.Get(row, j) - factor * M.Get(col, j));
        }
    }

    MutableArraySequence<T> x;
    for (int i = 0; i < n; i++) x.Append(T());
    for (int i = n - 1; i >= 0; i--) {
        T sum = M.Get(i, n);
        for (int j = i + 1; j < n; j++)
            sum = sum - M.Get(i, j) * x.Get(j);
        x.Set(i, sum / M.Get(i, i));
    }
    return x;
}

// Метод Гаусса с выбором ведущего по столбцу
template <class T>
MutableArraySequence<T> GaussPartialPivot(const SquareMatrix<T>& A,
    const MutableArraySequence<T>& b) {
    int n = A.Rows();
    if (A.Cols() != n || b.GetLength() != n)
        throw std::invalid_argument("Invalid dimensions");

    RectangularMatrix<T> M(n, n + 1);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            M.Set(i, j, A.Get(i, j));
        M.Set(i, n, b.Get(i));
    }

    for (int col = 0; col < n; col++) {
        int maxRow = col;
        T maxVal = std::abs((double)M.Get(col, col));
        for (int row = col + 1; row < n; row++) {
            T val = std::abs((double)M.Get(row, col));
            if (val > maxVal) { 
                maxVal = val;
                maxRow = row;
            }
        }
        if (maxVal < 1e-12)
            throw std::runtime_error("Matrix is singular");

        if (maxRow != col)
            M.SwapRows(col, maxRow);

        T pivot = M.Get(col, col);
        for (int row = col + 1; row < n; row++) {
            T factor = M.Get(row, col) / pivot;
            for (int j = col; j <= n; j++)
                M.Set(row, j, M.Get(row, j) - factor * M.Get(col, j));
        }
    }

    MutableArraySequence<T> x;
    for (int i = 0; i < n; i++) x.Append(T());
    for (int i = n - 1; i >= 0; i--) {
        T sum = M.Get(i, n);
        for (int j = i + 1; j < n; j++)
            sum = sum - M.Get(i, j) * x.Get(j);
        x.Set(i, sum / M.Get(i, i));
    }
    return x;
}

// A = L * U
template <class T>
LUResult<T> LUDecompose(const SquareMatrix<T>& A) {
    int n = A.Rows();
    if (A.Cols() != n)
        throw std::invalid_argument("Matrix must be square");

    LUResult<T> result(n);

    for (int i = 0; i < n; i++)
        result.L.Set(i, i, T(1));

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            T sum = A.Get(i, j);
            for (int k = 0; k < i; k++)
                sum = sum - result.L.Get(i, k) * result.U.Get(k, j);
            result.U.Set(i, j, sum);
        }
        for (int j = i + 1; j < n; j++) {
            T sum = A.Get(j, i);
            for (int k = 0; k < i; k++)
                sum = sum - result.L.Get(j, k) * result.U.Get(k, i);
            if (std::abs((double)result.U.Get(i, i)) < 1e-12)
                throw std::runtime_error("Zero pivot in LU decomposition");
            result.L.Set(j, i, sum / result.U.Get(i, i));
        }
    }
    return result;
}

// Решение СЛАУ через LU: A*x = b
template <class T>
MutableArraySequence<T> LUSolve(const SquareMatrix<T>& A,
    const MutableArraySequence<T>& b) {
    LUResult<T> lu = LUDecompose(A);
    MutableArraySequence<T> y = ForwardSubstitution(lu.L, b);
    return BackSubstitution(lu.U, y);
}

// Решение с уже готовым LU
template <class T>
MutableArraySequence<T> LUSolveDecomposed(const LUResult<T>& lu,
    const MutableArraySequence<T>& b) {
    MutableArraySequence<T> y = ForwardSubstitution(lu.L, b);
    return BackSubstitution(lu.U, y);
}