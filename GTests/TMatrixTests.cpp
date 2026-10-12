#include "pch.h"
#include "TMatrix.h"

TEST(TMatrix_Constructor, DefaultConstructor_Creates1x1Matrix) {
    TMatrix<int> test;
    EXPECT_EQ(test.size(), 1);
    EXPECT_EQ(test[0].size(), 1);
}

TEST(TMatrix_Constructor, SizeConstructor_CreatesMatrixOfGivenSize) {
    EXPECT_NO_THROW(TMatrix<int> test(5, 5));
    TMatrix<int> test2(3, 4);
    EXPECT_EQ(test2.size(), 3);
    EXPECT_EQ(test2[0].size(), 4);
}

TEST(TMatrix_Constructor, RawPointerConstructor_InitializesData) {
    int** data = new int* [3];
    int k = 0;
    for (int i = 0; i < 3; ++i) {
        data[i] = new int[5];
        for (int j = 0; j < 5; j++) {
            data[i][j] = ++k;
        }
    }

    TMatrix<int> test(3, 5, data);

    EXPECT_EQ(test(2, 3), 14);
    EXPECT_EQ(test(0, 0), 1);

    for (int i = 0; i < 3; ++i) delete[] data[i];
    delete[] data;
}

TEST(TMatrix_Constructor, InitializerListConstructor_InitializesData) {
    TMatrix<int> test{ std::array<int, 3>{3, 2, 1}, {5, 6, 7}, {13, 10, 2} };

    EXPECT_EQ(test(0, 2), 1);
    EXPECT_EQ(test(1, 1), 6);
    EXPECT_EQ(test(2, 0), 13);
}

TEST(TMatrix_Constructor, CopyConstructor_CopiesData) {
    TMatrix<int> test{ std::array<int, 4>{3, 2, 1, 7}, {5, 6, 7, 2}, {13, 10, 2, 11} };
    TMatrix<int> copy(test);

    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            EXPECT_EQ(test(i, j), copy(i, j));
        }
    }
}

TEST(TMatrix_Constructor, MoveConstructor_MovesData) {
    TMatrix<int> original{ std::array<int, 2>{1, 2}, {3, 4} };
    TMatrix<int> moved(std::move(original));

    EXPECT_EQ(moved(0, 0), 1);
    EXPECT_EQ(moved(1, 1), 4);
}

TEST(TMatrix_Accessors, ParenthesisOperator_ReturnsCorrectElement) {
    TMatrix<int> test{ std::array<int, 2>{10, 20}, {30, 40} };

    EXPECT_EQ(test(0, 0), 10);
    EXPECT_EQ(test(1, 1), 40);

    test(0, 1) = 99;
    EXPECT_EQ(test(0, 1), 99);
}

TEST(TMatrix_Accessors, ParenthesisOperator_ThrowsOnOutOfBounds) {
    TMatrix<int> test(2, 2);

    EXPECT_THROW(test(2, 0), std::out_of_range);
    EXPECT_THROW(test(0, 2), std::out_of_range);
    EXPECT_THROW(test(5, 5), std::out_of_range);
}

TEST(TMatrix_Methods, Transpose_TransposesSquareMatrix) {
    TMatrix<int> test{ std::array<int, 2>{1, 2}, {3, 4} };
    TMatrix<int> transposed = test.transpose();

    EXPECT_EQ(transposed(0, 0), 1);
    EXPECT_EQ(transposed(0, 1), 3);
    EXPECT_EQ(transposed(1, 0), 2);
    EXPECT_EQ(transposed(1, 1), 4);
}

TEST(TMatrix_Methods, Transpose_TransposesNonSquareMatrix) {
    TMatrix<int> test{ std::array<int, 4>{3, 2, 1, 7}, {5, 6, 7, 2}, {13, 10, 2, 11} };
    TMatrix<int> copy = test.transpose();

    EXPECT_EQ(copy.size(), 4);
    EXPECT_EQ(copy[0].size(), 3);

    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            EXPECT_EQ(test(i, j), copy(j, i));
        }
    }
}

TEST(TMatrix_Operators, PlusEquals_AddsMatricesCorrectly) {
    TMatrix<int> test1{ std::array<int, 3>{1, 1, 1}, {2, 2, 2}, {10, 10, 10} };
    TMatrix<int> test2{ std::array<int, 3>{2, 2, 2}, {3, 3, 3}, {5, 5, 5} };

    test1 += test2;

    EXPECT_EQ(test1(0, 1), 3);
    EXPECT_EQ(test1(1, 1), 5);
    EXPECT_EQ(test1(2, 1), 15);
}

TEST(TMatrix_Operators, CopyAssignment_AssignsCorrectly) {
    TMatrix<int> test1{ std::array<int, 2>{1, 2}, {3, 4} };
    TMatrix<int> test2;

    test2 = test1;

    EXPECT_EQ(test2(0, 0), 1);
    EXPECT_EQ(test2(1, 1), 4);
}

TEST(TMatrix_Operators, Multiplication_MultipliesNonSquareMatrices) {
    TMatrix<int> A = { std::array<int, 3>{1, 2, 3}, {4, 5, 6} };
    TMatrix<int> B = { std::array<int, 2>{7, 8}, {9, 10}, {11, 12} };

    // 2x3 * 3x2 = C 2x2
    TMatrix<int> C = A * B;

    EXPECT_EQ(C.size(), 2);
    EXPECT_EQ(C[0].size(), 2);

    EXPECT_EQ(C[0][0], 58);
    EXPECT_EQ(C[0][1], 64);
    EXPECT_EQ(C[1][0], 139);
    EXPECT_EQ(C[1][1], 154);
}

TEST(TMatrix_Operators, Multiplication_ThrowsOnInvalidDimensions) {
    TMatrix<int> A(2, 3);
    TMatrix<int> B(2, 2);

    EXPECT_THROW(A * B, std::invalid_argument);
}

TEST(TMatrix_Operators, Plus_AddsMatricesElementWise) {
    TMatrix<int> A = { std::array<int, 2>{1, 2}, {3, 4} };
    TMatrix<int> B = { std::array<int, 2>{5, 6}, {7, 8} };

    auto C = A + B;

    EXPECT_EQ(C[0][0], 6);
    EXPECT_EQ(C[0][1], 8);
    EXPECT_EQ(C[1][0], 10);
    EXPECT_EQ(C[1][1], 12);
}

TEST(TMatrix_Operators, Minus_SubtractsMatricesElementWise) {
    TMatrix<int> A = { std::array<int, 2>{5, 6}, {7, 8} };
    TMatrix<int> B = { std::array<int, 2>{1, 2}, {3, 4} };

    auto C = A - B;

    EXPECT_EQ(C[0][0], 4);
    EXPECT_EQ(C[0][1], 4);
    EXPECT_EQ(C[1][0], 4);
    EXPECT_EQ(C[1][1], 4);
}

TEST(TMatrix_Operators, ScalarMultiplication_MultipliesElementsByConstant) {
    TMatrix<int> A = { std::array<int, 2>{1, 2}, {3, 4} };

    TMatrix<int> C = A * 3.0;

    EXPECT_EQ(C[0][0], 3);
    EXPECT_EQ(C[0][1], 6);
    EXPECT_EQ(C[1][0], 9);
    EXPECT_EQ(C[1][1], 12);
}