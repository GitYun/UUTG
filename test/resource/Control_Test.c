#include "unity_fixture.h"

TEST_GROUP(Control);

TEST_SETUP(Control)
{

}

TEST_TEAR_DOWN(Control)
{

}

int *pointer1 = 0;
int *pointer2 = (int *)2;
int int1;
int int2;
int int3;

TEST(Control, Ramp)
{
    TEST_ASSERT_EQUAL_INT(1, 1);
    TEST_ASSERT_TRUE(1 == 1);
}

TEST(Control, Pointer)
{
    UT_PTR_SET(pointer1, &int1);
    UT_PTR_SET(pointer2, &int2);
    TEST_ASSERT_POINTERS_EQUAL(pointer1, &int1);
    TEST_ASSERT_POINTERS_EQUAL(pointer2, &int2);

    UT_PTR_SET(pointer1, &int3);
    // TEST_ASSERT_POINTERS_EQUAL(pointer1, 0);

    UnityPointer_UndoAllSets(); // 测试用例执行后，会自动执行一次

    TEST_ASSERT_POINTERS_EQUAL(pointer1, 0);
    TEST_ASSERT_POINTERS_EQUAL(pointer2, (int *)2);
}