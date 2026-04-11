#pragma once
#include "RectangularMatrix.h"
#include <cmath>


// Конструкторы
template <class T>
RectangularMatrix<T>::RectangularMatrix(int rows, int cols)
    : rows_(rows), cols_(cols), data_() {
    if (rows <= 0 || cols <= 0) {
        throw std::invalid_argument("Matrix dimensions must be positive");
    }
    for (int i = 0; i < rows * cols; i++) {
        data_.Append(T());
    }
}

template <class T>
RectangularMatrix<T>::RectangularMatrix(int rows, int cols, const T* data)
    : rows_(rows), cols_(cols), data_() {
    if (rows <= 0 || cols <= 0) {
        throw std::invalid_argument("Matrix dimensions must be positive");
    }
    if (data == nullptr)
        throw std::invalid_argument("Data array is null");
    for (int i = 0; i < rows * cols; i++) {
        data_.Append(data[i]);
    }
}


// Get / Set
template <class T>
const T& RectangularMatrix<T>::Get(int row, int col) const {
    this->CheckIndex(row, col);
    return data_.Get(row * cols_ + col);
}

template <class T>
void RectangularMatrix<T>::Set(int row, int col, const T& val) {
    this->CheckIndex(row, col);
    data_.Set(row * cols_ + col, val);
}

template <class T>
int RectangularMatrix<T>::Rows() const { return rows_; }

template <class T>
int RectangularMatrix<T>::Cols() const { return cols_; }


// Арифметика
template <class T>
IMatrix<T>* RectangularMatrix<T>::Add(const IMatrix<T>& other) const {
    this->CheckSameSize(other);
    auto* result = new RectangularMatrix<T>(rows_, cols_);
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            result->Set(i, j, Get(i, j) + other.Get(i, j));
        }
    }
    return result;
}

template <class T>
IMatrix<T>* RectangularMatrix<T>::MultiplyByScalar(const T& scalar) const {
    auto* result = new RectangularMatrix<T>(rows_, cols_);
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            result->Set(i, j, Get(i, j) * scalar);
        }
    }
    return result;
}

template <class T>
IMatrix<T>* RectangularMatrix<T>::MultiplyByMatrix(const IMatrix<T>& other) const {
    this->CheckMultiplySize(other);
    auto* result = new RectangularMatrix<T>(rows_, other.Cols());
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < other.Cols(); j++) {
            T sum = T();
            for (int k = 0; k < cols_; k++) {
                sum = sum + Get(i, k) * other.Get(k, j);
            }
            result->Set(i, j, sum);
        }
    }
    return result;
}

template <class T>
double RectangularMatrix<T>::FrobeniusNorm() const {
    double sum = 0.0;
    for (int i = 0; i < rows_ * cols_; i++) {
        double v = (double)data_.Get(i);
        sum += v * v;
    }
    return std::sqrt(sum);
}


// Элементарные преобразования строк
template <class T>
void RectangularMatrix<T>::SwapRows(int i, int j) {
    if (i < 0 || i >= rows_ || j < 0 || j >= rows_) {
        throw std::out_of_range("Row index out of range");
    }
    for (int k = 0; k < cols_; k++) {
        T tmp = Get(i, k);
        Set(i, k, Get(j, k));
        Set(j, k, tmp);
    }
}

template <class T>
void RectangularMatrix<T>::ScaleRow(int row, const T& scalar) {
    if (row < 0 || row >= rows_) {
        throw std::out_of_range("Row index out of range");
    }
    for (int j = 0; j < cols_; j++) {
        Set(row, j, Get(row, j) * scalar);
    }
}

template <class T>
void RectangularMatrix<T>::AddScaledRow(int targetRow, int sourceRow, const T& scalar) {
    if (targetRow < 0 || targetRow >= rows_ || sourceRow < 0 || sourceRow >= rows_) {
        throw std::out_of_range("Row index out of range");
    }
    for (int j = 0; j < cols_; j++) {
        Set(targetRow, j, Get(targetRow, j) + Get(sourceRow, j) * scalar);
    }
}


// Элементарные преобразования столбцов
template <class T>
void RectangularMatrix<T>::SwapCols(int i, int j) {
    if (i < 0 || i >= cols_ || j < 0 || j >= cols_) {
        throw std::out_of_range("Col index out of range");
    }
    for (int k = 0; k < rows_; k++) {
        T tmp = Get(k, i);
        Set(k, i, Get(k, j));
        Set(k, j, tmp);
    }
}

template <class T>
void RectangularMatrix<T>::ScaleCol(int col, const T& scalar) {
    if (col < 0 || col >= cols_) {
        throw std::out_of_range("Col index out of range");
    }
    for (int i = 0; i < rows_; i++) {
        Set(i, col, Get(i, col) * scalar);
    }
}

template <class T>
void RectangularMatrix<T>::AddScaledCol(int targetCol, int sourceCol, const T& scalar) {
    if (targetCol < 0 || targetCol >= cols_ || sourceCol < 0 || sourceCol >= cols_) {
        throw std::out_of_range("Col index out of range");
    }
    for (int i = 0; i < rows_; i++) {
        Set(i, targetCol, Get(i, targetCol) + Get(i, sourceCol) * scalar);
    }
}


// Транспонирование
template <class T>
IMatrix<T>* RectangularMatrix<T>::Transpose() const {
    auto* result = new RectangularMatrix<T>(cols_, rows_);
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            result->Set(j, i, Get(i, j));
        }
    }
    return result;
}