#include "unity_fixture.h"
#include "CCodeParse.h"
#include "UnitTestRunnerGenerator.h"

#define ROOT_PATH "F:\\StudyResource\\TDD\\TDDStudy\\UnityTestGenerator"

static char *g_pcCwd = ROOT_PATH "\\test\\resource";
static char ** g_ppsFileNameArray = NULL;
static int g_iFileCount = 0;

TEST_GROUP(RunnerGenerator);

TEST_SETUP(RunnerGenerator)
{
    g_ppsFileNameArray = NULL;
    g_iFileCount = -1;
}

TEST_TEAR_DOWN(RunnerGenerator)
{
    CCP_Free(&g_ppsFileNameArray, &g_iFileCount);
}

TEST(RunnerGenerator, Init)
{
    char * cwd = NULL;
    CCP_SetCwd(g_pcCwd);
    CCP_GetCwd(&cwd);

    TEST_ASSERT_EQUAL_STRING(g_pcCwd, cwd);
}

TEST(RunnerGenerator, GetFileName)
{
    char *filearray[] = {ROOT_PATH "\\test\\resource\\Control_Test.c",
                         ROOT_PATH "\\test\\resource\\DIDO_Test.c",
                         ROOT_PATH "\\test\\resource\\Startup_Test.c"};

    CCP_SetCwd(g_pcCwd);
    CCP_GetDirFileInfo(g_pcCwd, &g_ppsFileNameArray, &g_iFileCount, "_Test", ".c");

    TEST_ASSERT_NOT_EQUAL(-1, g_iFileCount);
    TEST_ASSERT_NOT_EQUAL(0, g_iFileCount);
    TEST_ASSERT_NOT_NULL(g_ppsFileNameArray);
    TEST_ASSERT_EQUAL_INT(3, g_iFileCount);
    TEST_ASSERT_EQUAL_STRING(filearray[0], g_ppsFileNameArray[0]);
    TEST_ASSERT_EQUAL_STRING(filearray[1], g_ppsFileNameArray[1]);
    TEST_ASSERT_EQUAL_STRING(filearray[2], g_ppsFileNameArray[2]);
    TEST_ASSERT_EQUAL_STRING_ARRAY(filearray, g_ppsFileNameArray, g_iFileCount);
}

TEST(RunnerGenerator, GetFileName_With_RelativePath)
{
    char *filearray[] = {ROOT_PATH "\\test\\resource\\Control_Test.c",
                         ROOT_PATH "\\test\\resource\\DIDO_Test.c",
                         ROOT_PATH "\\test\\resource\\Startup_Test.c"};

    CCP_SetCwd(ROOT_PATH);
    CCP_GetDirFileInfo("test\\resource", &g_ppsFileNameArray, &g_iFileCount, "_Test", ".c");

    TEST_ASSERT_NOT_EQUAL(-1, g_iFileCount);
    TEST_ASSERT_NOT_EQUAL(0, g_iFileCount);
    TEST_ASSERT_NOT_NULL(g_ppsFileNameArray);
    TEST_ASSERT_EQUAL_INT(3, g_iFileCount);
    TEST_ASSERT_EQUAL_STRING(filearray[0], g_ppsFileNameArray[0]);
    TEST_ASSERT_EQUAL_STRING(filearray[1], g_ppsFileNameArray[1]);
    TEST_ASSERT_EQUAL_STRING(filearray[2], g_ppsFileNameArray[2]);
    TEST_ASSERT_EQUAL_STRING_ARRAY(filearray, g_ppsFileNameArray, g_iFileCount);
}

TEST(RunnerGenerator, SubtraceString)
{
    char str1[] = "Hello World, Nice!";
    char str2[] = " ";
    char str3[1] = {'\0'};
    char *str4 = NULL;
    TEST_ASSERT_EQUAL_STRING("HelloWorld,Nice!", CCP_StringSubtract(str1, " \t"));
    TEST_ASSERT_EQUAL_STRING("HeoWord,Nice!", CCP_StringSubtract(str1, "l"));
    TEST_ASSERT_EQUAL_STRING(" ", CCP_StringSubtract(str2, "l"));
    TEST_ASSERT_EQUAL_STRING("", CCP_StringSubtract(str3, "l"));
    TEST_ASSERT_EQUAL_STRING(NULL, CCP_StringSubtract(str4, "l"));
}

TEST(RunnerGenerator, GetTest)
{
    char **test = NULL;
    int count = 0;
    CCP_SetCwd(g_pcCwd);
    CCP_GetDirFileInfo(g_pcCwd, &g_ppsFileNameArray, &g_iFileCount, "_Test", ".c");
    CCP_GetFuncNameFormSource(g_ppsFileNameArray, g_iFileCount, &test, &count);

    TEST_ASSERT_EQUAL(4, count);
    TEST_ASSERT_NOT_NULL(test);
#if 1
    TEST_ASSERT_EQUAL_STRING("TEST(Control,Ramp)", test[0]);
    TEST_ASSERT_EQUAL_STRING("TEST(Control,Pointer)", test[1]);
    TEST_ASSERT_EQUAL_STRING("IGNORE_TEST(DIDO,Output)", test[2]);
    TEST_ASSERT_EQUAL_STRING("TEST(Startup,EnableSys)", test[3]);
#endif

    CCP_Free(&test, &count);
    TEST_ASSERT_EQUAL(0, count);
    TEST_ASSERT_NULL(test);
}

TEST(RunnerGenerator, GetTestGroupAndTestName)
{
    char **test = NULL;
    int count = 0;
    CCP_SetCwd(g_pcCwd);
    CCP_GetDirFileInfo(g_pcCwd, &g_ppsFileNameArray, &g_iFileCount, "_Test", ".c");
    CCP_GetFuncNameFormSource(g_ppsFileNameArray, g_iFileCount, &test, &count);

    tTestGroup *group = NULL;

    int groupCount = RunnerGenerator_GetTestGroupAndTestname(test, count, &group);
    TEST_ASSERT_NOT_EQUAL(0, groupCount);
    TEST_ASSERT_NOT_NULL(group);
    TEST_ASSERT_EQUAL_INT(3, groupCount);
    TEST_ASSERT_EQUAL_INT(2, group[0].nameCount);
    TEST_ASSERT_EQUAL_INT(1, group[1].nameCount);
    TEST_ASSERT_EQUAL_INT(1, group[2].nameCount);

    TEST_ASSERT_EQUAL_STRING("Control", group[0].group);
    TEST_ASSERT_EQUAL_STRING("DIDO", group[1].group);
    TEST_ASSERT_EQUAL_STRING("Startup", group[2].group);
    TEST_ASSERT_EQUAL_STRING("Ramp", group[0].name[0]);
    TEST_ASSERT_EQUAL_STRING("Pointer", group[0].name[1]);
    TEST_ASSERT_EQUAL_STRING("Output", group[1].name[0]);
    TEST_ASSERT_EQUAL_STRING("EnableSys", group[2].name[0]);

    RunnerGenerator_TestGroupAndTestnameDestory(&group, groupCount);
    TEST_ASSERT_NULL(group);

    CCP_Free(&test, &count);
}

TEST(RunnerGenerator, GeneratorOutput)
{
    char **test = NULL;
    int count = 0;
    CCP_SetCwd(g_pcCwd);
    CCP_GetDirFileInfo(NULL, &g_ppsFileNameArray, &g_iFileCount, "_Test", ".c");
    CCP_GetFuncNameFormSource(g_ppsFileNameArray, g_iFileCount, &test, &count);
    CCP_PrintfMessage(g_ppsFileNameArray, g_iFileCount);
    CCP_PrintfMessage(test, count);

    tTestGroup *group = NULL;
    int groupCount = RunnerGenerator_GetTestGroupAndTestname(test, count, &group);
    RunnerGenerator_GeneratorRunnerFile(&group, groupCount, "UnityTestRunner.c");
    RunnerGenerator_GeneratorMainFile(&group, groupCount, g_pcCwd, "main_temp.c");

    PrintGroupMessage(&group, groupCount);

    RunnerGenerator_TestGroupAndTestnameDestory(&group, groupCount);
    TEST_ASSERT_NULL(group);

    CCP_Free(&test, &count);
}

TEST(RunnerGenerator, End)
{
    CCP_SetCwd(g_pcCwd);
    CCP_Free(&g_ppsFileNameArray, &g_iFileCount);
    TEST_ASSERT_EQUAL(-1, g_iFileCount);
    TEST_ASSERT_NULL(g_ppsFileNameArray);

    CCP_GetDirFileInfo(g_pcCwd, &g_ppsFileNameArray, &g_iFileCount, "_Test", ".c");
    TEST_ASSERT_NOT_EQUAL(-1, g_iFileCount);
    TEST_ASSERT_NOT_EQUAL(0, g_iFileCount);
    TEST_ASSERT_NOT_NULL(g_ppsFileNameArray);

    CCP_Free(&g_ppsFileNameArray, &g_iFileCount);
    TEST_ASSERT_NULL(g_ppsFileNameArray);
    TEST_ASSERT_EQUAL(0, g_iFileCount);
}

TEST_GROUP_RUNNER(RunnerGenerator)
{
    RUN_TEST_CASE(RunnerGenerator, Init);
    RUN_TEST_CASE(RunnerGenerator, GetFileName);
    RUN_TEST_CASE(RunnerGenerator, GetFileName_With_RelativePath);
    RUN_TEST_CASE(RunnerGenerator, SubtraceString);
    RUN_TEST_CASE(RunnerGenerator, GetTest);
    RUN_TEST_CASE(RunnerGenerator, GetTestGroupAndTestName);
    RUN_TEST_CASE(RunnerGenerator, GeneratorOutput);
    RUN_TEST_CASE(RunnerGenerator, End);
}
