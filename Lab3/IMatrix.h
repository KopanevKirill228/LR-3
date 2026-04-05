#pragma once
#include <stdexcept>

template <class T>
class IMatrix {
public:
    virtual const T& Get(int row, int col) const = 0;
    virtual void Set(int row, int col, const T& val) = 0;
    virtual int Rows() const = 0;
    virtual int Cols() const = 0;

    virtual IMatrix<T>* Add(const IMatrix<T>& other) const = 0;
    virtual IMatrix<T>* MultiplyByScalar(const T& scalar) const = 0;
    virtual IMatrix<T>* MultiplyByMatrix(const IMatrix<T>& other) const = 0;
    virtual double FrobeniusNorm() const = 0;

    virtual void SwapRows(int i, int j) = 0;
    virtual void ScaleRow(int row, const T& scalar) = 0;
    virtual void AddScaledRow(int rowToModify, int rowToAdd, const T& scalar);
    virtual void SwapCols(int i, int j) = 0;
    virtual void ScaleCol(int col, const T& scalar) = 0;
    virtual void AddScaledCol(int colToModify, int colToAdd, const T& scalar);

    virtual IMatrix<T>* Transpose() const = 0;

    virtual ~IMatrix() = default;

protected:
    void CheckIndex(int row, int col) const {
        if (row < 0 || row >= Rows())
            throw std::out_of_range("Row index out of range");
        if (col < 0 || col >= Cols())
            throw std::out_of_range("Col index out of range");
    }

    void CheckSameSize(const IMatrix<T>& other) const {
        if (Rows() != other.Rows() || Cols() != other.Cols())
            throw std::invalid_argument("Matrix sizes do not match");
    }

    void CheckMultiplySize(const IMatrix<T>& other) const {
        if (Cols() != other.Rows())
            throw std::invalid_argument("Matrix sizes incompatible for multiplication");
    }
};