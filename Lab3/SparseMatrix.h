#pragma once
#include "IMatrix.h"
#include "lib/ArraySequence.h"
#include <cmath>

template <class T>
struct SparseElement {
    int row, col;
    T value;
};

template <class T>
class SparseMatrix : public IMatrix<T> {
public:
    SparseMatrix(int rows, int cols);
    SparseMatrix(const SparseMatrix<T>& other) = default;
    ~SparseMatrix() override = default;

    T Get(int row, int col) const override;
    void Set(int row, int col, T val) override;
    int Rows() const override;
    int Cols() const override;

    IMatrix<T>* Add(const IMatrix<T>& other) const override;
    IMatrix<T>* MulScalar(T scalar) const override;
    IMatrix<T>* MulMatrix(const IMatrix<T>& other) const override;
    double Norm() const override;

    void SwapRows(int i, int j) override;
    void MulRow(int i, T scalar) override;
    void AddRow(int dst, int src, T scalar) override;
    void SwapCols(int i, int j) override;
    void MulCol(int j, T scalar) override;
    void AddCol(int dst, int src, T scalar) override;

    IMatrix<T>* Transpose() const override;

    int NonZeroCount() const;

private:
    int rows_, cols_;
    MutableArraySequence<SparseElement<T>> data_;

    int FindIndex(int row, int col) const;
};

#include "SparseMatrix.tpp"