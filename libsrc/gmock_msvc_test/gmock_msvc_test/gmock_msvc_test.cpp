// gmock_msvc_test.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <gmock\gmock.h>
#include <gtest\gtest.h>


int Factorial(int n)
{
	int Result = 1;
	for (int i = 1; i <= n; i++)
	{
		Result *= i;
	}

	return Result;
}

TEST(Factorial, Zero)
{
	EXPECT_EQ(1, Factorial(0));
}


TEST(Factorial, Positive)
{
	EXPECT_EQ(1, Factorial(1));
	EXPECT_EQ(120, Factorial(5));
	EXPECT_EQ(3628800, Factorial(10));
	EXPECT_EQ(720, Factorial(6));
	EXPECT_EQ(5040, Factorial(7));
}



int _tmain(int argc, _TCHAR* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

