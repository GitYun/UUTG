#include "unity_fixture.h"

TEST_GROUP(DIDO);

TEST_SETUP(DIDO)
{
}

TEST_TEAR_DOWN(DIDO)
{
}

IGNORE_TEST(DIDO, Output)
{
    TEST_ASSERT_EQUAL_INT(2, 1);
}