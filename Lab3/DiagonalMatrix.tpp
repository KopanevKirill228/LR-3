#pragma once
#include "DiagonalMatrix.h"

template <class T>
DiagonalMatrix<T>::DiagonalMatrix(int n)
    : SquareMatrix<T>(n) {}

template <class T>
DiagonalMatrix<T>::DiagonalMatrix(int n, T* diag)
    : SquareMatrix<T>(n) {
    for (int i = 0; i < n; i++)
        SquareMatrix<T>::Set(i, i, diag[i]);
}

template <class T>
T DiagonalMatrix<T>::Get(int row, int col) const {
    this->CheckIndex(row, col, this->Rows(), this->Cols());
    return (row == col) ? SquareMatrix<T>::Get(row, col) : T();
}

template <class T>
void DiagonalMatrix<T>::Set(int row, int col, T val) {
    this->CheckIndex(row, col, this->Rows(), this->Cols());
    if (row != col)
        throw std::invalid_argument("DiagonalMatrix: only diagonal elements can be set");
    SquareMatrix<T>::Set(row, col, val);
}

template <class T>
T DiagonalMatrix<T>::GetDiag(int i) const {
    if (i < 0 || i >= this->Rows()) throw std::out_of_range("Diagonal index out of range");
    return SquareMatrix<T>::Get(i, i);
}

template <class T>
void DiagonalMatrix<T>::SetDiag(int i, T val) {
    if (i < 0 || i >= this->Rows()) throw std::out_of_range("Diagonal index out of range");
    SquareMatrix<T>::Set(i, i, val);
}

template <class T>
DiagonalMatrix<T>* DiagonalMatrix<T>::Inverse() const {
    int n = this->Rows();
    for (int i = 0; i < n; i++)
        if (SquareMatrix<T>::Get(i, i) == T())
            throw std::runtime_error("DiagonalMatrix: matrix is singular, cannot invert");
    auto* result = new DiagonalMatrix<T>(n);
    for (int i = 0; i < n; i++)
        result->SetDiag(i, T(1) / SquareMatrix<T>::Get(i, i));
    return result;
}

template <class T>
DiagonalMatrix<T>* DiagonalMatrix<T>::MulDiag(const DiagonalMatrix<T>& other) const {
    int n = this->Rows();
    if (n != other.Rows())
        throw std::invalid_argument("DiagonalMatrix: dimensions do not match");
    auto* result = new DiagonalMatrix<T>(n);
    for (int i = 0; i < n; i++)
        result->SetDiag(i, SquareMatrix<T>::Get(i, i) * other.GetDiag(i));
    return result;
}