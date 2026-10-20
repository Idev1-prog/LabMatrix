#pragma once
#include <iostream>
#include "TMathVector.h"
// Нижняя треугольная матрицы
template <typename T>
class TTriangleMatrix : public TMathVector<TMathVector<T>> {
private:
    size_t _rows;
    size_t _cols;
public:
    TTriangleMatrix(size_t rows = 1, size_t columns = 1);
    TTriangleMatrix(size_t columns,std::initializer_list<std::initializer_list<T>> init);
    TTriangleMatrix(std::initializer_list<std::initializer_list<T>> init) : TTriangleMatrix(init.size(), init) {}
    TTriangleMatrix(size_t rows, size_t columns, T** data);
    TTriangleMatrix(const TTriangleMatrix<T>& other) = default;
    TTriangleMatrix(TTriangleMatrix<T>&& other) noexcept = default;
    ~TTriangleMatrix() = default;

    size_t rows() const noexcept { return _rows; }
    size_t cols() const noexcept { return _cols; }

    T& operator()(size_t row, size_t col);
    T at(size_t row, size_t col) const;

    TTriangleMatrix<T> operator*(double val) const;
    TTriangleMatrix<T> operator+(const TTriangleMatrix<T>& other) const;
    TTriangleMatrix<T> operator-(const TTriangleMatrix<T>& other) const;
    TTriangleMatrix<T> operator*(const TTriangleMatrix<T>& other) const;

    TTriangleMatrix& operator=(const TTriangleMatrix&) = default;
    TTriangleMatrix& operator=(TTriangleMatrix&&) noexcept = default;

};

template <typename T>
TTriangleMatrix<T>::TTriangleMatrix(size_t rows, size_t columns) : TMathVector<TMathVector<T>>(rows), _rows(rows), _cols(columns) {
    if (_cols == 0) throw std::invalid_argument("An attempt to create a matrix of size 0");
    if (rows > columns)
        throw std::invalid_argument("Lower triangular matrix requires rows <= columns");

    for (size_t i = 0; i < _rows; ++i)
        (*this)[i] = TMathVector<T>(i + 1);
}

template <typename T>
TTriangleMatrix<T>::TTriangleMatrix(size_t columns, std::initializer_list<std::initializer_list<T>> init): TMathVector<TMathVector<T>>(init.size()), _rows(init.size()), _cols(columns) { // возможно нужно переделать
    if (init.size() == 0)
        throw std::invalid_argument("Empty initializer list");
    if (_rows > _cols)
        throw std::invalid_argument("Rows must be <= columns");

    size_t expected = 1;
    for (const auto& row : init) {
        if (row.size() != expected)
            throw std::invalid_argument("Violation of the string initialization order (string i must be one unit larger in size than i-1)");
        ++expected;
    }

    size_t i = 0;
    for (const auto& row : init)
        (*this)[i++] = TMathVector<T>(row);
}

template <typename T>
TTriangleMatrix<T>::TTriangleMatrix(size_t rows, size_t columns, T** data) : TTriangleMatrix(rows, columns) { // Без проверки валидности входных данных
    if (data == nullptr)
        throw std::invalid_argument("Data is null");

    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j <= i; ++j) {
            (*this)[i][j] = data[i][j];
        }
    }
}

template <typename T>
T& TTriangleMatrix<T>::operator()(size_t row, size_t col) {
    if (row >= _rows || col >= _cols) {
        throw std::out_of_range("Matrix index out of bounds");
    }
    if (col > row) 
        throw std::logic_error("TTriangleMatrix: cannot write above diagonal");
    return (*this)[row][col];
}

template <typename T>
T TTriangleMatrix<T>::at(size_t row, size_t col) const {
    if (row >= _rows || col >= _cols)
        throw std::out_of_range("TriangleMatrix index out of bounds");
    return (col > row) ? T{} : (*this)[row][col];
}

template<typename T>
TTriangleMatrix<T> TTriangleMatrix<T>::operator*(double val) const {
    TTriangleMatrix<T> copy(*this);
    copy *= val;
    return copy;
}

template<typename T>
TTriangleMatrix<T> TTriangleMatrix<T>::operator+(const TTriangleMatrix<T>& other) const {
    TTriangleMatrix<T> copy(*this);
    copy += other;
    return copy;
}

template<typename T>
TTriangleMatrix<T> TTriangleMatrix<T>::operator-(const TTriangleMatrix<T>& other) const {
    TTriangleMatrix<T> copy(*this);
    copy -= other;
    return copy;
}

template<typename T>
TTriangleMatrix<T> TTriangleMatrix<T>::operator*(const TTriangleMatrix<T>& other) const {
    if (_cols != other._rows)
        throw std::invalid_argument("TriangleMatrix left cols must equal right rows");

    TTriangleMatrix<T> result(_rows, other._cols);

    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j <= i; ++j) {
            T sum = T{};
            for (size_t k = j; k <= i; ++k)
                sum += (*this)[i][k] * other[k][j];
            result[i][j] = sum;
        }
    }
    return result;
}