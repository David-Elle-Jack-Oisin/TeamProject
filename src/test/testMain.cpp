#include "gtest/gtest.h"

TEST (OneEqualOneTest, oneEqual) { 
    ASSERT_EQ (1,1);
}

int main( int argc, const char *argv[] )
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}