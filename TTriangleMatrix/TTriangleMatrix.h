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

    TTriangleMatrix& operator=(const TTriangleMatrix&) = default;
    TTriangleMatrix& operator=(TTriangleMatrix&&) noexcept = default;
};

template <typename T>
TTriangleMatrix<T>::TTriangleMatrix(size_t rows, size_t columns) : TMathVector<TMathVector<T>>(rows), _rows(rows), _cols(columns) {
    if (_cols == 0) throw std::invalid_argument("An attempt to create a matrix of size 0.");
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