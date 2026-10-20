#include "pch.h"
#include "TTriangleMatrix.h"
#include <sstream>
#include <stdexcept>

TEST(TTriangleMatrixConstruct, DefaultConstruction) {
    TTriangleMatrix<int> m;  // 1x1
    EXPECT_EQ(m.rows(), 1);
    EXPECT_EQ(m.cols(), 1);
}

TEST(TTriangleMatrixConstruct, SizedConstruction) {
    TTriangleMatrix<int> m(3, 10);  // 3x10
    EXPECT_EQ(m.rows(), 3);
    EXPECT_EQ(m.cols(), 10);
}

TEST(TTriangleMatrixConstruct, InitializerList) {
    TTriangleMatrix<int> m = {
        {3},
        {4, 5},
        {4, 6, 1}
    };  // 3x3

    EXPECT_EQ(m.at(0, 0), 3);
    EXPECT_EQ(m.at(1, 0), 4);
    EXPECT_EQ(m.at(1, 1), 5);
    EXPECT_EQ(m.at(2, 0), 4);
    EXPECT_EQ(m.at(2, 1), 6);
    EXPECT_EQ(m.at(2, 2), 1);
}

TEST(TTriangleMatrixConstruct, InitializerListWrongShapeThrows) {
    EXPECT_THROW(
        TTriangleMatrix<int>({ {1, 2}, {3, 4} }),
        std::invalid_argument
    );
}

TEST(TTriangleMatrixConstruct, FromData) {
    // Матрица 3x4, значения 1-12
    int** data = new int* [3];
    int k = 0;
    for (int i = 0; i < 3; ++i) {
        data[i] = new int[4];
        for (int j = 0; j < 4; ++j)
            data[i][j] = ++k;
    }

    TTriangleMatrix<int> m(3, 4, data);
    // Ожидаемое содержимое:
    //   1  0  0  0
    //   5  6  0  0
    //   9 10 11  0

    std::stringstream out;
    for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 4; ++j)
            out << m.at(i, j);
    EXPECT_EQ("10005600910110", out.str());

    for (int i = 0; i < 3; ++i) delete[] data[i];
    delete[] data;
}


TEST(TTriangleMatrixAccess, ReadVirtualZero) {
    TTriangleMatrix<int> m({ {3}, {4, 5}, {4, 6, 1} });  // 3x3

    EXPECT_EQ(m.at(0, 1), 0);
    EXPECT_EQ(m.at(0, 2), 0);
    EXPECT_EQ(m.at(1, 2), 0);
    EXPECT_ANY_THROW(m(0, 1));
}

TEST(TTriangleMatrixAccess, AtOutOfRangeThrows) {
    TTriangleMatrix<int> m({ {1}, {2, 3} });  // 2x2

    EXPECT_THROW(m.at(0, 2), std::out_of_range);
    EXPECT_THROW(m.at(2, 0), std::out_of_range);
    EXPECT_THROW(m.at(2, 2), std::out_of_range);
}

TEST(TTriangleMatrixAccess, WriteAboveDiagonalThrows) {
    TTriangleMatrix<int> m({ {1}, {2, 3} });  // 2x2
    EXPECT_ANY_THROW(m(0, 1) = 5);
}

TEST(TTriangleMatrixAccess, WriteBelowDiagonal) {
    TTriangleMatrix<int> m({ {1}, {2, 3} });  // 2x2

    m(0, 0) = 10;
    m(1, 0) = 20;
    m(1, 1) = 30;

    EXPECT_EQ(m.at(0, 0), 10);
    EXPECT_EQ(m.at(1, 0), 20);
    EXPECT_EQ(m.at(1, 1), 30);
}

TEST(TTriangleMatrixAdd, AddAssignBasic) {
    TTriangleMatrix<int> a({ {1}, {2, 3}, {4, 5, 6} });       // 3x3
    TTriangleMatrix<int> b({ {10}, {20, 30}, {40, 50, 60} }); // 3x3

    a += b;

    EXPECT_EQ(a.at(0, 0), 11);
    EXPECT_EQ(a.at(0, 1), 0);
    EXPECT_EQ(a.at(1, 0), 22);
    EXPECT_EQ(a.at(1, 1), 33);
    EXPECT_EQ(a.at(1, 2), 0);
    EXPECT_EQ(a.at(2, 0), 44);
    EXPECT_EQ(a.at(2, 1), 55);
    EXPECT_EQ(a.at(2, 2), 66);
}

TEST(TTriangleMatrixAdd, AddBasic) {
    TTriangleMatrix<int> a({ {1}, {2, 3} });    // 2x2
    TTriangleMatrix<int> b({ {10}, {20, 30} }); // 2x2

    auto c = a + b;

    EXPECT_EQ(c[0][0], 11);
    EXPECT_EQ(c[1][0], 22);
    EXPECT_EQ(c[1][1], 33);
}

TEST(TTriangleMatrixAdd, SubAssignBasic) {
    TTriangleMatrix<int> a({ {10}, {20, 30} }); // 2x2
    TTriangleMatrix<int> b({ {1}, {2, 3} });    // 2x2

    a -= b;

    EXPECT_EQ(a.at(0, 0), 9);
    EXPECT_EQ(a.at(0, 1), 0);
    EXPECT_EQ(a.at(1, 0), 18);
    EXPECT_EQ(a.at(1, 1), 27);
}

TEST(TTriangleMatrixAdd, SubBasic) {
    TTriangleMatrix<int> a({ {10}, {20, 30} }); // 2x2
    TTriangleMatrix<int> b({ {1}, {2, 3} });    // 2x2

    auto c = a - b;

    EXPECT_EQ(c[0][0], 9);
    EXPECT_EQ(c[1][0], 18);
    EXPECT_EQ(c[1][1], 27);
}

TEST(TTriangleMatrixAdd, MismatchedSizesThrows) {
    TTriangleMatrix<int> a({ {1}, {2, 3} });              // 2x2
    TTriangleMatrix<int> b({ {1}, {2, 3}, {4, 5, 6} });   // 3x3

    EXPECT_THROW(a += b, std::invalid_argument);
    EXPECT_THROW(a -= b, std::invalid_argument);
}

TEST(TTriangleMatrixAdd, SingleElement) {
    TTriangleMatrix<int> a({ {5} });  // 1x1
    TTriangleMatrix<int> b({ {3} });  // 1x1

    a += b;
    EXPECT_EQ(a.at(0, 0), 8);

    a -= b;
    EXPECT_EQ(a.at(0, 0), 5);
}

TEST(TTriangleMatrixMul, Basic) {
    TTriangleMatrix<int> a({ {1}, {2, 3} });  // 2x2
    TTriangleMatrix<int> b({ {4}, {5, 6} });  // 2x2
    // C = A·B = [[4, 0], [23, 18]]

    TTriangleMatrix<int> c = a * b;

    EXPECT_EQ(c.at(0, 0), 4);
    EXPECT_EQ(c.at(0, 1), 0);
    EXPECT_EQ(c.at(1, 0), 23);
    EXPECT_EQ(c.at(1, 1), 18);
}

TEST(TTriangleMatrixMul, Rectangular) {
    TTriangleMatrix<int> a(2, 3);  // 2x3
    a(0, 0) = 1;
    a(1, 0) = 2; a(1, 1) = 3;

    TTriangleMatrix<int> b(3, 4);  // 3x4
    b(0, 0) = 1;
    b(1, 0) = 2; b(1, 1) = 3;
    b(2, 0) = 4; b(2, 1) = 5; b(2, 2) = 6;

    TTriangleMatrix<int> c = a * b;  // 2x4

    EXPECT_EQ(c.at(0, 0), 1);
    EXPECT_EQ(c.at(0, 1), 0);
    EXPECT_EQ(c.at(1, 0), 8);
    EXPECT_EQ(c.at(1, 1), 9);
}

TEST(TTriangleMatrixMul, DimMismatchThrows) {
    TTriangleMatrix<int> a({ {1}, {2, 3} });              // 2x2
    TTriangleMatrix<int> b({ {1}, {2, 3}, {4, 5, 6} });   // 3x3

    EXPECT_THROW(a * b, std::invalid_argument);
}