#pragma once
#include "IMatrix.h"
#include <ostream>

template <class T>
IMatrix<T>* operator+(const IMatrix<T>& a, const IMatrix<T>& b) {
    return a.Add(b);
}

template <class T>
IMatrix<T>* operator*(const IMatrix<T>& m, T scalar) {
    return m.MulScalar(scalar);
}

template <class T>
IMatrix<T>* operator*(T scalar, const IMatrix<T>& m) {
    return m.MulScalar(scalar);
}

template <class T>
IMatrix<T>* operator*(const IMatrix<T>& a, const IMatrix<T>& b) {
    return a.MulMatrix(b);
}

template <class T>
bool operator==(const IMatrix<T>& a, const IMatrix<T>& b) {
    if (a.Rows() != b.Rows() || a.Cols() != b.Cols()) return false;
    for (int i = 0; i < a.Rows(); i++)
        for (int j = 0; j < a.Cols(); j++)
            if (a.Get(i, j) != b.Get(i, j)) return false;
    return true;
}

template <class T>
bool operator!=(const IMatrix<T>& a, const IMatrix<T>& b) {
    return !(a == b);
}

template <class T>
std::ostream& operator<<(std::ostream& os, const IMatrix<T>& m) {
    for (int i = 0; i < m.Rows(); i++) {
        for (int j = 0; j < m.Cols(); j++)
            os << m.Get(i, j) << " ";
        os << "\n";
    }
    return os;
}