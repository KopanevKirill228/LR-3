#pragma once
#include "DiagonalMatrix.h"
#include <stdexcept>
#include <cmath>
#include "RectangularMatrix.h"


//  онструкторы
template <class T>
DiagonalMatrix<T>::DiagonalMatrix(int n)
    : n(n), diag_() {
    if (n <= 0) {
        throw std::invalid_argument("Matrix dimension must be positive");
    }
    for (int i = 0; i < n; i++) {
        diag_.Append(T());
    }
}

template <class T>
DiagonalMatrix<T>::DiagonalMatrix(int n, const T* diag)
    : n(n), diag_() {
    if (n <= 0)
        throw std::invalid_argument("Matrix dimension must be positive");
    if (diag == nullptr)
        throw std::invalid_argument("Diagonal array is null");
    for (int i = 0; i < n; i++)
        diag_.Append(diag[i]);
}


// Get / Set
template <class T>
const T& DiagonalMatrix<T>::Get(int row, int col) const {
    this->CheckIndex(row, col);
    if (row == col) {
        return diag_.Get(row);
    }
    static const T zero{};
    return zero;
}

template <class T>
void DiagonalMatrix<T>::Set(int row, int col, const T& val) {
    this->CheckIndex(row, col);
    if (row != col) {
        if (val != T()) {
            throw std::invalid_argument("Cannot set non-zero value outside diagonal");
        }
        return;
    }
    diag_.Set(row, val);
}

template <class T>
int DiagonalMatrix<T>::Rows() const { return n; }

template <class T>
int DiagonalMatrix<T>::Cols() const { return n; }


// ƒиагональные методы
template <class T>
const T& DiagonalMatrix<T>::GetDiag(int i) const {
    if (i < 0 || i >= n) {
        throw std::out_of_range("Diagonal index out of range");
    }
    return diag_.Get(i);
}

template <class T>
void DiagonalMatrix<T>::SetDiag(int i, const T& val) {
    if (i < 0 || i >= n) {
        throw std::out_of_range("Diagonal index out of range");
    }
    diag_.Set(i, val);
}


// јрифметика
template <class T>
IMatrix<T>* DiagonalMatrix<T>::Add(const IMatrix<T>& other) const {
    this->CheckSameSize(other);
    auto* result = new DiagonalMatrix<T>(n);
    for (int i = 0; i < n; i++) {
        result->SetDiag(i, diag_.Get(i) + other.Get(i, i));
    }
    return result;
}

template <class T>
IMatrix<T>* DiagonalMatrix<T>::MultiplyByScalar(const T& scalar) const {
    auto* result = new DiagonalMatrix<T>(n);
    for (int i = 0; i < n; i++) {
        result->SetDiag(i, diag_.Get(i) * scalar);
    }
    return result;
}

template <class T>
IMatrix<T>* DiagonalMatrix<T>::MultiplyByMatrix(const IMatrix<T>& other) const {
    this->CheckMultiplySize(other);
    auto* result = new RectangularMatrix<T>(n, other.Cols());
    for (int i = 0; i < n; i++)
        for (int j = 0; j < other.Cols(); j++)
            result->Set(i, j, diag_.Get(i) * other.Get(i, j));
    return result;
}

template <class T>
double DiagonalMatrix<T>::FrobeniusNorm() const {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        double v = (double)diag_.Get(i);
        sum += v * v;
    }
    return std::sqrt(sum);
}


// Ёлементарные преобразовани€ строк
template <class T>
void DiagonalMatrix<T>::SwapRows(int i, int j) {
    if (i < 0 || i >= n || j < 0 || j >= n) {
        throw std::out_of_range("Row index out of range");
    }
    T tmp = diag_.Get(i);
    diag_.Set(i, diag_.Get(j));
    diag_.Set(j, tmp);
}

template <class T>
void DiagonalMatrix<T>::ScaleRow(int row, const T& scalar) {
    if (row < 0 || row >= n) {
        throw std::out_of_range("Row index out of range");
    }
    diag_.Set(row, diag_.Get(row) * scalar);
}

template <class T>
void DiagonalMatrix<T>::AddScaledRow(int targetRow, int sourceRow, const T& scalar) {
    if (targetRow < 0 || targetRow >= n || sourceRow < 0 || sourceRow >= n) {
        throw std::out_of_range("Row index out of range");
    }
    // ƒл€ диагональной матрицы операци€ сложени€ строк нарушает диагональность
    if (targetRow != sourceRow) {
        throw std::invalid_argument("AddScaledRow breaks diagonal structure");
    }
    diag_.Set(targetRow, diag_.Get(targetRow) + diag_.Get(sourceRow) * scalar);
}


// Ёлементарные преобразовани€ столбцов
template <class T>
void DiagonalMatrix<T>::SwapCols(int i, int j) {
    // ƒл€ диагональной матрицы SwapCols идентична SwapRows
    SwapRows(i, j);
}

template <class T>
void DiagonalMatrix<T>::ScaleCol(int col, const T& scalar) {
    if (col < 0 || col >= n) {
        throw std::out_of_range("Col index out of range");
    }
    diag_.Set(col, diag_.Get(col) * scalar);
}

template <class T>
void DiagonalMatrix<T>::AddScaledCol(int targetCol, int sourceCol, const T& scalar) {
    if (targetCol < 0 || targetCol >= n || sourceCol < 0 || sourceCol >= n) {
        throw std::out_of_range("Col index out of range");
    }
    if (targetCol != sourceCol) {
        throw std::invalid_argument("AddScaledCol breaks diagonal structure");
    }
    diag_.Set(targetCol, diag_.Get(targetCol) + diag_.Get(sourceCol) * scalar);
}


// “ранспонирование
template <class T>
IMatrix<T>* DiagonalMatrix<T>::Transpose() const {
    // ƒиагональна€ матрица симметрична Ч транспонирование возвращает копию
    return new DiagonalMatrix<T>(*this);
}


// ќбратна€ матрица
template <class T>
DiagonalMatrix<T>* DiagonalMatrix<T>::Inverse() const {
    auto* result = new DiagonalMatrix<T>(n);
    for (int i = 0; i < n; i++) {
        if (diag_.Get(i) == T()) {
            delete result;
            throw std::runtime_error("Diagonal element is zero Ч matrix is singular");
        }
        result->SetDiag(i, T(1) / diag_.Get(i));
    }
    return result;
}


// ”множение двух диагональных матриц
template <class T>
DiagonalMatrix<T>* DiagonalMatrix<T>::MultiplyDiagonal(const DiagonalMatrix<T>& other) const {
    if (n != other.n) {
        throw std::invalid_argument("Matrix sizes do not match");
    }
    auto* result = new DiagonalMatrix<T>(n);
    for (int i = 0; i < n; i++) {
        result->SetDiag(i, diag_.Get(i) * other.diag_.Get(i));
    }
    return result;
}