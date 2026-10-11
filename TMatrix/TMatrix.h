#pragma once
#include "TMathVector.h"


template <typename T>
class TMatrix : public TMathVector<TMathVector<T>> {
private:
    unsigned int _rows;
    unsigned int _cols;
public:
    TMatrix(size_t rows = 1, size_t columns = 1) : TMathVector<TMathVector<T>>(rows), _rows(rows), _cols(columns) {
        for (size_t i = 0; i < rows; ++i) {
            (*this)[i] = TMathVector<T>(columns);
        }
    }
    TMatrix(size_t rows, size_t colums, T** data);
    TMatrix(std::initializer_list<std::initializer_list<T>> init);
    TMatrix(const TMatrix<T>& other) = default; // ?
    TMatrix(TMatrix<T>&& other) noexcept = default; // ?
    ~TMatrix() = default; // ?

    T& operator()(size_t row, size_t col);
    const T& operator()(size_t row, size_t col) const;
};

template <typename T>
T& TMatrix<T>::operator()(size_t row, size_t col) {
    if (row >= this->size() || col >= (*this)[row].size()) {
        throw std::out_of_range("Matrix index out of bounds");
    }
    return (*this)[row][col];
}

template <typename T>
const T& TMatrix<T>::operator()(size_t row, size_t col) const {
    if (row >= this->size() || col >= (*this)[row].size()) {
        throw std::out_of_range("Matrix index out of bounds");
    }
    return (*this)[row][col];
}

template <typename T>
TMatrix<T>::TMatrix(size_t rows, size_t colums, T** data) : TMatrix(rows, colums) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < colums; ++j) {
            (*this)(i, j) = data[i][j];
        }
    }
}