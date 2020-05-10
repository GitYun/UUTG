#include "unity_fixture.h"
#include "uutg.h"

#define ROOT_PATH "F:\\WorkPath\\ProgramCode\\C\\UnitTestGenerator"
static char *g_pcCwd = ROOT_PATH "\\inc";//"\\test\\resource"; //
static char *g_pcPwd = NULL;

TEST_GROUP(UnitTestGenerator);

TEST_SETUP(UnitTestGenerator)
{

}

TEST_TEAR_DOWN(UnitTestGenerator)
{

}

TEST(UnitTestGenerator, UUTG)
{
    char *newDir = ROOT_PATH "/test/AutoGen";
    UnitTestGenerator(g_pcCwd, newDir, ".h");
    TEST_ASSERT_EQUAL_INT(1, 1);
}

TEST_GROUP_RUNNER(UnitTestGenerator)
{
    RUN_TEST_CASE(UnitTestGenerator, UUTG);
}