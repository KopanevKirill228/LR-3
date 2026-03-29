#pragma once
#include "SquareMatrix.h"

template <class T>
class DiagonalMatrix : public SquareMatrix<T> {
public:
    DiagonalMatrix(int n);
    DiagonalMatrix(int n, T* diag);
    DiagonalMatrix(const DiagonalMatrix<T>& other) = default;
    ~DiagonalMatrix() override = default;

    T Get(int row, int col) const override;
    void Set(int row, int col, T val) override;

    T GetDiag(int i) const;
    void SetDiag(int i, T val);
    DiagonalMatrix<T>* Inverse() const;
    DiagonalMatrix<T>* MulDiag(const DiagonalMatrix<T>& other) const;
};

#include "DiagonalMatrix.tpp"