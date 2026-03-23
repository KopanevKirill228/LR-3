#pragma once
#include "IMatrix.h"
#include "lib/Dynamic_Array.h"
#include <cmath>

template <class T>
class RectangularMatrix : public IMatrix<T> {
public:
    RectangularMatrix(int rows, int cols);
    RectangularMatrix(int rows, int cols, T** data);
    RectangularMatrix(const RectangularMatrix<T>& other);
    ~RectangularMatrix() override;

    T    Get(int row, int col) const override;
    void Set(int row, int col, T val) override;
    int  Rows() const override;
    int  Cols() const override;

    IMatrix<T>* Add(const IMatrix<T>& other) const override;
    IMatrix<T>* MulScalar(T scalar) const override;
    IMatrix<T>* MulMatrix(const IMatrix<T>& other) const override;
    double      Norm() const override;

    void SwapRows(int i, int j) override;
    void MulRow(int i, T scalar) override;
    void AddRow(int dst, int src, T scalar) override;
    void SwapCols(int i, int j) override;
    void MulCol(int j, T scalar) override;
    void AddCol(int dst, int src, T scalar) override;

    IMatrix<T>* Transpose() const override;

private:
    int rows_, cols_;
    DynamicArray<T>* data_; // хранение: data_[row * cols_ + col]
};

#include "RectangularMatrix.tpp"