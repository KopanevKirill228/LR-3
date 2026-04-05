#pragma once
#include "SparseMatrix.h"

// Конструктор
template <class T>
SparseMatrix<T>::SparseMatrix(int rows, int cols)
    : rows_(rows), cols_(cols), data_() {
    if (rows <= 0 || cols <= 0) {
        throw std::invalid_argument("Matrix dimensions must be positive");
    }
}

// Get / Set / Rows / Cols
template <class T>
const T& SparseMatrix<T>::Get(int row, int col) const {
    this->CheckIndex(row, col);
    int idx = FindIndex(row, col);
    if (idx == -1) {
        static const T zero{};
        return zero;
    }
    return data_.Get(idx).value;
}

template <class T>
void SparseMatrix<T>::Set(int row, int col, const T& val) {
    this->CheckIndex(row, col);
    int idx = FindIndex(row, col);

    if (val == T()) {
        // Записываем ноль — удаляем элемент если он есть
        if (idx != -1) {
            data_.RemoveAt(idx);
        }
    }
    else {
        if (idx != -1) {
            // Обновляем существующий
            SparseElement<T> el = data_.Get(idx);
            el.value = val;
            data_.Set(idx, el);
        }
        else {
            // Добавляем новый
            data_.Append({ row, col, val });
        }
    }
}

template <class T>
int SparseMatrix<T>::Rows() const { return rows_; }

template <class T>
int SparseMatrix<T>::Cols() const { return cols_; }

template <class T>
int SparseMatrix<T>::NonZeroCount() const { return data_.GetLength(); }

// Арифметика
template <class T>
IMatrix<T>* SparseMatrix<T>::Add(const IMatrix<T>& other) const {
    this->CheckSameSize(other);
    auto* result = new SparseMatrix<T>(rows_, cols_);
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            result->Set(i, j, Get(i, j) + other.Get(i, j));
        }
    }
    return result;
}

template <class T>
IMatrix<T>* SparseMatrix<T>::MultiplyByScalar(const T& scalar) const {
    auto* result = new SparseMatrix<T>(rows_, cols_);
    // Умножаем только ненулевые
    for (int i = 0; i < data_.GetLength(); i++) {
        const SparseElement<T>& el = data_.Get(i);
        result->Set(el.row, el.col, el.value * scalar);
    }
    return result;
}

template <class T>
IMatrix<T>* SparseMatrix<T>::MultiplyByMatrix(const IMatrix<T>& other) const {
    this->CheckMultiplySize(other);
    auto* result = new SparseMatrix<T>(rows_, other.Cols());
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
double SparseMatrix<T>::FrobeniusNorm() const {
    double sum = 0.0;
    for (int i = 0; i < data_.GetLength(); i++) {
        double v = (double)data_.Get(i).value;
        sum += v * v;
    }
    return std::sqrt(sum);
}

// Элементарные преобразования строк
template <class T>
void SparseMatrix<T>::SwapRows(int i, int j) {
    if (i < 0 || i >= rows_ || j < 0 || j >= rows_) {
        throw std::out_of_range("Row index out of range");
    }
    for (int k = 0; k < data_.GetLength(); k++) {
        SparseElement<T> el = data_.Get(k);
        if (el.row == i) {
            el.row = j;
            data_.Set(k, el);
        }
        else if (el.row == j) {
            el.row = i;
            data_.Set(k, el);
        }
    }
}

template <class T>
void SparseMatrix<T>::ScaleRow(int row, const T& scalar) {
    if (row < 0 || row >= rows_) {
        throw std::out_of_range("Row index out of range");
    }
    if (scalar == T()) {
        // Умножение на ноль — удаляем все элементы строки
        for (int k = data_.GetLength() - 1; k >= 0; k--) {
            if (data_.Get(k).row == row) {
                data_.RemoveAt(k);
            }
        }
    }
    else {
        for (int k = 0; k < data_.GetLength(); k++) {
            SparseElement<T> el = data_.Get(k);
            if (el.row == row) {
                el.value = el.value * scalar;
                data_.Set(k, el);
            }
        }
    }
}

template <class T>
void SparseMatrix<T>::AddScaledRow(int targetRow, int sourceRow, const T& scalar) {
    if (targetRow < 0 || targetRow >= rows_ || sourceRow < 0 || sourceRow >= rows_) {
        throw std::out_of_range("Row index out of range");
    }
    for (int j = 0; j < cols_; j++) {
        T val = Get(targetRow, j) + Get(sourceRow, j) * scalar;
        Set(targetRow, j, val);
    }
}

// Элементарные преобразования столбцов
template <class T>
void SparseMatrix<T>::SwapCols(int i, int j) {
    if (i < 0 || i >= cols_ || j < 0 || j >= cols_) {
        throw std::out_of_range("Col index out of range");
    }
    for (int k = 0; k < data_.GetLength(); k++) {
        SparseElement<T> el = data_.Get(k);
        if (el.col == i) {
            el.col = j;
            data_.Set(k, el);
        }
        else if (el.col == j) {
            el.col = i;
            data_.Set(k, el);
        }
    }
}

template <class T>
void SparseMatrix<T>::ScaleCol(int col, const T& scalar) {
    if (col < 0 || col >= cols_) {
        throw std::out_of_range("Col index out of range");
    }
    if (scalar == T()) {
        for (int k = data_.GetLength() - 1; k >= 0; k--) {
            if (data_.Get(k).col == col) {
                data_.RemoveAt(k);
            }
        }
    }
    else {
        for (int k = 0; k < data_.GetLength(); k++) {
            SparseElement<T> el = data_.Get(k);
            if (el.col == col) {
                el.value = el.value * scalar;
                data_.Set(k, el);
            }
        }
    }
}

template <class T>
void SparseMatrix<T>::AddScaledCol(int targetCol, int sourceCol, const T& scalar) {
    if (targetCol < 0 || targetCol >= cols_ || sourceCol < 0 || sourceCol >= cols_) {
        throw std::out_of_range("Col index out of range");
    }
    for (int i = 0; i < rows_; i++) {
        T val = Get(i, targetCol) + Get(i, sourceCol) * scalar;
        Set(i, targetCol, val);
    }
}

// Транспонирование
template <class T>
IMatrix<T>* SparseMatrix<T>::Transpose() const {
    auto* result = new SparseMatrix<T>(cols_, rows_);
    // Транспонируем только ненулевые
    for (int i = 0; i < data_.GetLength(); i++) {
        const SparseElement<T>& el = data_.Get(i);
        result->Set(el.col, el.row, el.value);
    }
    return result;
}

template <class T>
int SparseMatrix<T>::FindIndex(int row, int col) const {
    for (int i = 0; i < data_.GetLength(); i++) {
        if (data_.Get(i).row == row && data_.Get(i).col == col) {
            return i;
        }
    }
    return -1;
}