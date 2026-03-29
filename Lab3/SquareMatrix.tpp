#pragma once
#include "SquareMatrix.h"

template <class T>
SquareMatrix<T>::SquareMatrix(int n)
    : RectangularMatrix<T>(n, n) {}

template <class T>
SquareMatrix<T>::SquareMatrix(int n, T** data)
    : RectangularMatrix<T>(n, n, data) {}

template <class T>
T SquareMatrix<T>::Trace() const {
    T result = T();
    for (int i = 0; i < this->Rows(); i++)
        result = result + this->Get(i, i);
    return result;
}

template <class T>
T SquareMatrix<T>::Determinant() const {
    int n = this->Rows();
    SquareMatrix<T> temp(*this);
    T det = T(1);
    for (int col = 0; col < n; col++) {
        int pivot = -1;
        for (int row = col; row < n; row++) {
            if (temp.Get(row, col) != T()) { pivot = row; break; }
        }
        if (pivot == -1) return T();  // вырожденная
        if (pivot != col) {
            temp.SwapRows(col, pivot);
            det = det * T(-1);
        }
        det = det * temp.Get(col, col);
        for (int row = col + 1; row < n; row++) {
            T factor = temp.Get(row, col) / temp.Get(col, col);
            temp.AddRow(row, col, T(-1) * factor);
        }
    }
    return det;
}

template <class T>
SquareMatrix<T>* SquareMatrix<T>::Power(int p) const {
    if (p < 0)
        throw std::invalid_argument("SquareMatrix: negative power not supported");
    int n = this->Rows();
    auto* result = new SquareMatrix<T>(n);
    for (int i = 0; i < n; i++) result->Set(i, i, T(1));
    SquareMatrix<T> base(*this);
    while (p > 0) {
        if (p % 2 == 1) {
            auto* tmp = static_cast<SquareMatrix<T>*>(result->MulMatrix(base));
            delete result;
            result = tmp;
        }
        auto* tmp = static_cast<SquareMatrix<T>*>(base.MulMatrix(base));
        base = *tmp;
        delete tmp;
        p /= 2;
    }
    return result;
}