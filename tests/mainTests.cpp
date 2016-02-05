#include "../src/tests/tests.hpp"
#include "../clients/pc/src/tests/tests.hpp"

#include <gtest/gtest.h>

int main( int argc, char *argv[] )
{
    testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}