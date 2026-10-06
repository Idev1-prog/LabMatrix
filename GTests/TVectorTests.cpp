#include "pch.h"
#include "TMemdata.h"
#include "TVector.h"

#define TVECTOR_TEST

#ifdef TVECTOR_TEST

TEST(MemDataInt, DefaultConstructor) {
    MemData<int> md;
    EXPECT_EQ(md.size(), 0);
    EXPECT_TRUE(md.is_empty());
}

TEST(MemDataInt, ConstructorWithSize) {
    MemData<int> md(10);
    EXPECT_EQ(md.size(), 10);
    EXPECT_GE(md.capacity(), 10);
}

TEST(MemDataInt, InitializerList) {
    MemData<int> md = { 1, 2, 3, 4, 5 };
    EXPECT_EQ(md.size(), 5);
    EXPECT_EQ(md.data()[2], 3);
}

TEST(MemDataInt, CopyConstructor) {
    MemData<int> original = { 1, 2, 3 };
    MemData<int> copy(original);
    EXPECT_EQ(copy.size(), 3);
    EXPECT_NE(copy.data(), original.data());
    EXPECT_EQ(copy.data()[0], 1);
}

TEST(TVectorInt, DefaultConstructor) {
    TVector<int> vec;
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), MEM_STEP);
}

TEST(TVectorInt, PushBackAndAccess) {
    TVector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);
    EXPECT_EQ(vec[2], 30);
}

TEST(TVectorInt, PushFront) {
    TVector<int> vec;
    vec.push_back(2);
    vec.push_front(1);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
}

TEST(TVectorInt, PopOperations) {
    TVector<int> vec = { 1, 2, 3 };
    vec.pop_front();
    EXPECT_EQ(vec[0], 2);
    vec.pop_back();
    EXPECT_EQ(vec.back(), 2);
}

TEST(TVectorInt, InsertAndErase) {
    TVector<int> vec = { 1, 3, 4 };
    vec.insert(2, 1);
    EXPECT_EQ(vec[1], 2);
    vec.erase(1);
    EXPECT_EQ(vec[1], 3);
}

TEST(TVectorInt, OutputOperator) {
    TVector<int> vec = { 1, 2, 3 };
    std::stringstream ss;
    ss << vec;
    EXPECT_EQ(ss.str(), "{ 1, 2, 3 }");
}

TEST(TVectorInt, CircularBufferBehavior) {
    TVector<int> vec;
    for (int i = 0; i < MEM_STEP; ++i) vec.push_back(i);
    for (int i = 0; i < 5; ++i) vec.pop_front();
    vec.push_back(100);
    vec.push_back(101);

    EXPECT_EQ(vec.size(), MEM_STEP + 2 - 5);
    EXPECT_EQ(vec[0], 5);
    EXPECT_EQ(vec.back(), 101);
}

TEST(TVectorDouble, DoublePrecision) {
    TVector<double> vec;
    vec.push_back(1.1);
    vec.push_back(2.2);
    EXPECT_DOUBLE_EQ(vec[0], 1.1);
    EXPECT_DOUBLE_EQ(vec[1], 2.2);
}

TEST(TVectorDouble, InputOperator) {
    TVector<double> vec;
    std::stringstream in("3 1.5 2.5 3.5");
    in >> vec;
    EXPECT_EQ(vec.size(), 3);
    EXPECT_DOUBLE_EQ(vec[0], 1.5);
    EXPECT_DOUBLE_EQ(vec[2], 3.5);
}

TEST(TVectorShrinkToFit, BaseTest) {
    TVector<int> vec;
    int* data = new int[5];
    for (int i = 0; i < 5; ++i) data[i] = i;
    vec.push_back_few(data, 5);
    vec.shrink_to_fit();
    for (int i = 0; i < 5; ++i) EXPECT_EQ(data[i], vec[i]);
    EXPECT_EQ(vec.capacity(), vec.size());
}

TEST(TVectorShrinkToFit, PostOperativeTest) {
    TVector<int> vec;
    int* data = new int[5];
    for (int i = 0; i < 5; ++i) data[i] = i;
    vec.push_back_few(data, 5);
    vec.push_front(11); // 11 0 1 2 3 4
    vec.pop_back_few(2); // 11 0 1 2
    vec.push_front(6); // 6 11 0 1 2
    vec.pop_back();// 6 11 0 1
    vec.push_back(8); // 6 11 0 1 8
    vec.shrink_to_fit();
    std::ostringstream out;
    out << vec;
    EXPECT_EQ(out.str(), "{ 6, 11, 0, 1, 8 }");
    EXPECT_EQ(vec.capacity(), vec.size());
}

#endif // TVECTOR_TEST