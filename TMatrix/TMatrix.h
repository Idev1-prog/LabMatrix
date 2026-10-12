#pragma once
#include "TMathVector.h"
#include <array>


template <typename T>
class TMatrix : public TMathVector<TMathVector<T>> {
private:
    size_t _rows;
    size_t _cols;
public:
    TMatrix(size_t rows = 1, size_t columns = 1) : TMathVector<TMathVector<T>>(rows), _rows(rows), _cols(columns) {
        for (size_t i = 0; i < rows; ++i) {
            (*this)[i] = TMathVector<T>(columns);
        }
    }
    TMatrix(size_t rows, size_t colums, T** data);
    template <size_t N>
    TMatrix(std::initializer_list<std::array<T, N>> init);
    TMatrix(const TMatrix<T>& other) = default;
    TMatrix(TMatrix<T>&& other) noexcept = default;
    ~TMatrix() = default;

    TMatrix<T>& operator=(const TMatrix<T>& other) noexcept = default;
    TMatrix<T>& operator=(TMatrix<T>&& other) noexcept = default;

    T& operator()(size_t row, size_t col);
    const T& operator()(size_t row, size_t col) const;

    TMatrix<T> transpose() const;
    TMatrix<T> operator*(const TMatrix<T>& other) const;
    TMatrix<T> operator*(const T& scalar) const;
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

template <typename T>
template <size_t N>
TMatrix<T>::TMatrix(std::initializer_list<std::array<T, N>> init) : TMatrix(init.size(), N) {
    size_t row_index = 0;
    for (auto row_data = init.begin() ; row_data != init.end(); ++row_data) {
        for (int col_index = 0; col_index < _cols; ++col_index) {
            (*this)(row_index, col_index) = (*row_data)[col_index];
        }
        row_index++;
    }
}

template <typename T>
TMatrix<T> TMatrix<T>::transpose() const {
    TMatrix<T> transposed(_cols, _rows);
    for (int i = 0; i < _rows; ++i) {
        for (int j = 0; j < _cols; ++j) {
            transposed(j, i) = (*this)(i, j);
        }
    }
    return transposed;
}

template <typename T>
TMatrix<T> TMatrix<T>::operator*(const TMatrix<T>& other) const {
    if (this->_cols != other._rows) throw std::invalid_argument("Matrix: invalid dimensions for multiplication");
    TMatrix<T> transposed_other = other.transpose();
    TMatrix<T> res(_rows, other._cols);
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < other._cols; j++) {
            res[i][j] = (*this)[i] * transposed_other[j];
        }
    }
    return res;
}

template <typename T>
TMatrix<T> TMatrix<T>::operator*(const T& scalar) const {
    TMatrix<T> result(*this);
    for (size_t i = 0; i < this->size(); ++i) {
        for (size_t j = 0; j < (*this)[i].size(); ++j) {
            result[i][j] *= scalar;
        }
    }
    return result;
}
