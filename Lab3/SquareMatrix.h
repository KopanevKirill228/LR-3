#pragma once
#include "RectangularMatrix.h"

template <class T>
class SquareMatrix : public RectangularMatrix<T> {
public:
    SquareMatrix(int n);
    SquareMatrix(int n, T* data);
    SquareMatrix(const SquareMatrix<T>& other) = default;
    ~SquareMatrix() override = default;

    T Determinant() const;
    T Trace() const;
};

#include "SquareMatrix.tpp"