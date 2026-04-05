#pragma once
#include "IMatrix.h"
#include "lib/ArraySequence.h"

template <class T>
class DiagonalMatrix : public IMatrix<T> {
public:
    explicit DiagonalMatrix(int n);
    DiagonalMatrix(int n, const T* diag);
    DiagonalMatrix(const DiagonalMatrix<T>& other) = default;
    ~DiagonalMatrix() override = default;

    const T& Get(int row, int col) const override;
    void Set(int row, int col, const T& val) override;
    int Rows() const override;
    int Cols() const override;

    IMatrix<T>* Add(const IMatrix<T>& other) const override;
    IMatrix<T>* MultiplyByScalar(const T& scalar) const override;
    IMatrix<T>* MultiplyByMatrix(const IMatrix<T>& other) const override;
    double FrobeniusNorm() const override;

    void SwapRows(int i, int j) override;
    void ScaleRow(int row, const T& scalar) override;
    void AddScaledRow(int targetRow, int sourceRow, const T& scalar) override;
    void SwapCols(int i, int j) override;
    void ScaleCol(int col, const T& scalar) override;
    void AddScaledCol(int targetCol, int sourceCol, const T& scalar) override;

    IMatrix<T>* Transpose() const override;

    const T& GetDiag(int i) const;
    void SetDiag(int i, const T& val);
    DiagonalMatrix<T>* Inverse() const;
    DiagonalMatrix<T>* MultiplyDiagonal(const DiagonalMatrix<T>& other) const;

private:
    int n;
    MutableArraySequence<T> diag_;
};

#include "DiagonalMatrix.tpp"