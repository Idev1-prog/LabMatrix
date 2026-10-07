#include "pch.h"
#include "TMathVector.h"

#define TMATHVECTOR_TEST

#ifdef TMATHVECTOR_TEST
template <typename T>
class TMathVectorTest : public ::testing::Test {};
using TMathVectorTestTypes = ::testing::Types<int, double>;
TYPED_TEST_CASE(TMathVectorTest, TMathVectorTestTypes);

TYPED_TEST(TMathVectorTest, Constructor_InitializerList) {
    TMathVector<TypeParam> v({ 1, 2, 3 });
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TYPED_TEST(TMathVectorTest, Constructor_PointerAndSize) {
    TypeParam data[] = { 4, 5, 6 };
    TMathVector<TypeParam> v(3, data);
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 4);
    EXPECT_EQ(v[1], 5);
    EXPECT_EQ(v[2], 6);
}

TYPED_TEST(TMathVectorTest, Constructor_Copy) {
    TMathVector<TypeParam> v1({ 1, 2, 3 });
    TMathVector<TypeParam> v2(v1);

    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2[0], 1);
    EXPECT_EQ(v2[1], 2);
    EXPECT_EQ(v2[2], 3);

    v1[0] = 99;
    EXPECT_EQ(v2[0], 1);
}

TYPED_TEST(TMathVectorTest, Constructor_Move) {
    TMathVector<TypeParam> v1({ 1, 2, 3 });
    TMathVector<TypeParam> v2(std::move(v1));

    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2[0], 1);
    EXPECT_EQ(v2[1], 2);
    EXPECT_EQ(v2[2], 3);
}

TYPED_TEST(TMathVectorTest, Assignment_Copy) {
    TMathVector<TypeParam> v1({ 1, 2, 3 });
    TMathVector<TypeParam> v2({ 0, 0, 0 });

    v2 = v1;

    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2[0], 1);
    EXPECT_EQ(v2[1], 2);
    EXPECT_EQ(v2[2], 3);
}

TYPED_TEST(TMathVectorTest, Assignment_Move) {
    TMathVector<TypeParam> v1({ 1, 2, 3 });
    TMathVector<TypeParam> v2({ 0, 0, 0 });

    v2 = std::move(v1);

    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2[0], 1);
    EXPECT_EQ(v2[1], 2);
    EXPECT_EQ(v2[2], 3);
}

TYPED_TEST(TMathVectorTest, Operator_AdditionAssignment) {
    TMathVector<TypeParam> v1({ 1, 2, 3 });
    TMathVector<TypeParam> v2({ 4, 5, 6 });

    v1 += v2;

    EXPECT_EQ(v1[0], 5);
    EXPECT_EQ(v1[1], 7);
    EXPECT_EQ(v1[2], 9);
}

TYPED_TEST(TMathVectorTest, Operator_Addition) {
    TMathVector<TypeParam> v1({ 1, 2, 3 });
    TMathVector<TypeParam> v2({ 4, 5, 6 });

    TMathVector<TypeParam> v3 = v1 + v2;

    EXPECT_EQ(v3[0], 5);
    EXPECT_EQ(v3[1], 7);
    EXPECT_EQ(v3[2], 9);
    EXPECT_EQ(v1[0], 1);
    EXPECT_EQ(v2[0], 4);
}

TYPED_TEST(TMathVectorTest, Operator_SubtractionAssignment) {
    TMathVector<TypeParam> v1({ 5, 7, 9 });
    TMathVector<TypeParam> v2({ 4, 5, 6 });

    v1 -= v2;

    EXPECT_EQ(v1[0], 1);
    EXPECT_EQ(v1[1], 2);
    EXPECT_EQ(v1[2], 3);
}

TYPED_TEST(TMathVectorTest, Operator_Subtraction) {
    TMathVector<TypeParam> v1({ 5, 7, 9 });
    TMathVector<TypeParam> v2({ 4, 5, 6 });

    TMathVector<TypeParam> v3 = v1 - v2;

    EXPECT_EQ(v3[0], 1);
    EXPECT_EQ(v3[1], 2);
    EXPECT_EQ(v3[2], 3);
}

TYPED_TEST(TMathVectorTest, Operator_MultiplicationAssignment_Scalar) {
    TMathVector<TypeParam> v({ 1, 2, 3 });

    v *= 2.0;

    EXPECT_EQ(v[0], 2);
    EXPECT_EQ(v[1], 4);
    EXPECT_EQ(v[2], 6);
}

TYPED_TEST(TMathVectorTest, Operator_Multiplication_Scalar) {
    TMathVector<TypeParam> v1({ 1, 2, 3 });

    TMathVector<TypeParam> v2 = v1 * 2.0;

    EXPECT_EQ(v2[0], 2);
    EXPECT_EQ(v2[1], 4);
    EXPECT_EQ(v2[2], 6);
    EXPECT_EQ(v1[0], 1);
}

TYPED_TEST(TMathVectorTest, Operator_DotProduct) {
    TMathVector<TypeParam> v1({ 2, 4, 6 });
    TMathVector<TypeParam> v2({ 4, 8, 12 });

    TypeParam result = v1 * v2; // 2*4 + 4*8 + 6*12 = 8 + 32 + 72 = 112

    if constexpr (std::is_floating_point_v<TypeParam>) {
        EXPECT_DOUBLE_EQ(result, 112.0);
    }
    else {
        EXPECT_EQ(result, 112);
    }
}

TYPED_TEST(TMathVectorTest, Operator_Equal_True) {
    TMathVector<TypeParam> v1({ 1, 2, 3 });
    TMathVector<TypeParam> v2({ 1, 2, 3 });

    EXPECT_TRUE(v1 == v2);
}

TYPED_TEST(TMathVectorTest, Operator_Equal_False) {
    TMathVector<TypeParam> v1({ 1, 2, 3 });
    TMathVector<TypeParam> v2({ 1, 2, 4 });

    EXPECT_FALSE(v1 == v2);
}

TYPED_TEST(TMathVectorTest, Operator_NotEqual) {
    TMathVector<TypeParam> v1({ 1, 2, 3 });
    TMathVector<TypeParam> v2({ 1, 2, 4 });
    TMathVector<TypeParam> v3({ 1, 2, 3 });

    EXPECT_TRUE(v1 != v2);
    EXPECT_FALSE(v1 != v3);
}

TYPED_TEST(TMathVectorTest, Exception_Addition_SizeMismatch) {
    TMathVector<TypeParam> v1({ 1, 2 });
    TMathVector<TypeParam> v2({ 1, 2, 3 });

    EXPECT_THROW(v1 + v2, std::invalid_argument);
    EXPECT_THROW(v1 += v2, std::invalid_argument);
}

TYPED_TEST(TMathVectorTest, Exception_Subtraction_SizeMismatch) {
    TMathVector<TypeParam> v1({ 1, 2 });
    TMathVector<TypeParam> v2({ 1, 2, 3 });

    EXPECT_THROW(v1 - v2, std::invalid_argument);
    EXPECT_THROW(v1 -= v2, std::invalid_argument);
}

TYPED_TEST(TMathVectorTest, Exception_DotProduct_SizeMismatch) {
    TMathVector<TypeParam> v1({ 1, 2 });
    TMathVector<TypeParam> v2({ 1, 2, 3 });

    EXPECT_THROW(v1 * v2, std::invalid_argument);
}

TYPED_TEST(TMathVectorTest, Exception_Comparison_SizeMismatch) {
    TMathVector<TypeParam> v1({ 1, 2 });
    TMathVector<TypeParam> v2({ 1, 2, 3 });

    EXPECT_THROW(v1 == v2, std::invalid_argument);
    EXPECT_THROW(v1 != v2, std::invalid_argument);
}

TEST(TMathVectorStreamTest, Output_Stream) {
    TMathVector<int> v({ 1, 2, 3 });
    std::ostringstream oss;

    oss << v;

    EXPECT_EQ(oss.str(), "{ 1, 2, 3 }");
}

TEST(TMathVectorStreamTest, Input_Stream) {
    int dummy[] = { 99 };
    TMathVector<int> v(1, dummy);

    std::istringstream iss("3 10 20 30");

    iss >> v;

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
}
#endif