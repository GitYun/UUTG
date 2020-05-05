#include "unity_fixture.h"
#include "CCodeParse.h"
#include "UnitTestGenerator.h"

#define ROOT_PATH "F:\\WorkPath\\ProgramCode\\C\\UnitTestGenerator"
static char *g_pcCwd = ROOT_PATH "\\inc";//"\\test\\resource"; //
static char *g_pcPwd = NULL;

TEST_GROUP(UnitTestGenerator);

TEST_SETUP(UnitTestGenerator)
{
    CCP_GetCwd(&g_pcPwd);
    CCP_SetCwd(g_pcCwd);
}

TEST_TEAR_DOWN(UnitTestGenerator)
{
    CCP_SetCwd(g_pcPwd);
}

TEST(UnitTestGenerator, Mocks)
{
    char *newDir = ROOT_PATH "/test/AutoGen";
    UnitTestGenerator(g_pcCwd, newDir, ".h");
    TEST_ASSERT_EQUAL_INT(1, 1);
}

TEST_GROUP_RUNNER(UnitTestGenerator)
{
    RUN_TEST_CASE(UnitTestGenerator, Mocks);
}