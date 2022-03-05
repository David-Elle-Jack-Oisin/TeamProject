#include "gtest/gtest.h"

TEST (Control_Flow_Tests, oneEqual) { 
    ASSERT_EQ (1,1);
}

int main( int argc, const char *argv[] )
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}