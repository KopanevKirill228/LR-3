#pragma once
#include <stdexcept>

template <class T>
class IMatrix {
public:
    virtual T Get(int row, int col) const = 0;
    virtual void Set(int row, int col, T val) = 0;
    virtual int Rows() const = 0;
    virtual int Cols() const = 0;

    virtual IMatrix<T>* Add(const IMatrix<T>& other) const = 0;
    virtual IMatrix<T>* MulScalar(T scalar) const = 0;
    virtual IMatrix<T>* MulMatrix(const IMatrix<T>& other) const = 0;

    // Норма Фробениуса
    virtual double Norm() const = 0;

    virtual void SwapRows(int i, int j) = 0;
    virtual void MulRow(int i, T scalar) = 0;
    virtual void AddRow(int dst, int src, T scalar) = 0;

    virtual void SwapCols(int i, int j) = 0;
    virtual void MulCol(int j, T scalar) = 0;
    virtual void AddCol(int dst, int src, T scalar) = 0;

    virtual IMatrix<T>* Transpose() const = 0;

    virtual ~IMatrix() = default;

protected:
    void CheckIndex(int row, int col, int rows, int cols) const {
        if (row < 0 || row >= rows)
            throw std::out_of_range("Row index out of range");
        if (col < 0 || col >= cols)
            throw std::out_of_range("Col index out of range");
    }

    void CheckSameSize(const IMatrix<T>& other) const {
        if (Rows() != other.Rows() || Cols() != other.Cols())
            throw std::invalid_argument("Matrix sizes do not match");
    }

    void CheckMulSize(const IMatrix<T>& other) const {
        if (Cols() != other.Rows())
            throw std::invalid_argument("Matrix sizes incompatible for multiplication");
    }
};