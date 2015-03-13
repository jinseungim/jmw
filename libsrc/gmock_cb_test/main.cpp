#include <iostream>
#include <gmock\gmock.h>
#include <gtest\gtest.h>
using namespace std;



int Factorial(int n)
{
    int Result = 1 ;
    for(int i = 1; i <= n; i++)
    {
        Result *= i ;
    }

    return Result ;
}

TEST(Factorial, Zero)
{
    EXPECT_EQ(1, Factorial(0)) ;
}


TEST(Factorial, Positive)
{
    EXPECT_EQ(1, Factorial(1)) ;
    EXPECT_EQ(120, Factorial(5)) ;
    EXPECT_EQ(3628800, Factorial(10)) ;
    EXPECT_EQ(720, Factorial(6)) ;
    EXPECT_EQ(5040, Factorial(7)) ;
}




int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv) ;
    return RUN_ALL_TESTS();
}
