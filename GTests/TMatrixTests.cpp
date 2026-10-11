#include "pch.h"
#include "TMatrix.h"

TEST(TestEx, TestEx1) {
	EXPECT_NO_THROW(TMatrix<int> test(5, 5));
}

TEST(TestEx, TestEx2) {
	int** data = new int*[3];
	int k = 0;
	for (int i = 0; i < 3; ++i) data[i] = new int[5];
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 5; j++) {
			data[i][j] = ++k;
		}
	}
	TMatrix<int> test(3, 5, data);
	EXPECT_EQ(test(2, 3), 14);
}