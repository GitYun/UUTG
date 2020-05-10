#include "unity_fixture.h"
#include <string.h>
#include "uutg_spy.h"

TEST_GROUP(GeneratorOutputSpy);

TEST_SETUP(GeneratorOutputSpy)
{
    OutputSpy_Create();
}

TEST_TEAR_DOWN(GeneratorOutputSpy)
{
    OutputSpy_Destory();
}

TEST(GeneratorOutputSpy, Create)
{
    TEST_ASSERT_NOT_NULL(OutputSpy_GetLineArray());
    TEST_ASSERT_EQUAL_INT(0, OutputSpy_GetLineCount());
    TEST_ASSERT_NULL(OutputSpy_GetLine(0));
    TEST_ASSERT_EQUAL_INT(0, OutputSpy_GetCharCount());
}

TEST(GeneratorOutputSpy, Destory)
{
    OutputSpy_Destory();
    TEST_ASSERT_NULL(OutputSpy_GetLineArray());
    TEST_ASSERT_NULL(OutputSpy_GetLine(0));
    TEST_ASSERT_EQUAL_INT(0, OutputSpy_GetLineCount());
    TEST_ASSERT_EQUAL_INT(0, OutputSpy_GetCharCount());
}

TEST(GeneratorOutputSpy, WriteNULL)
{
    char str[] = "";

    fwrite_spy(str, 1, strlen(str), NULL);
    TEST_ASSERT_EQUAL_INT(0, OutputSpy_GetLineCount());
    TEST_ASSERT_EQUAL_INT(0, OutputSpy_GetCharCount());

    fwrite_spy(NULL, 1, 0, NULL);
    TEST_ASSERT_EQUAL_INT(0, OutputSpy_GetLineCount());
    TEST_ASSERT_EQUAL_INT(0, OutputSpy_GetCharCount());
}

TEST(GeneratorOutputSpy, WriteOneLine)
{
    char str[] = "\nhello world\nnihao\n";
    fwrite_spy(str, 1, strlen(str), NULL);
    
    TEST_ASSERT_EQUAL_INT(3, OutputSpy_GetLineCount());
    TEST_ASSERT_EQUAL_STRING("\n", OutputSpy_GetLine(0));
    TEST_ASSERT_EQUAL_STRING("hello world\n", OutputSpy_GetLine(1));
    TEST_ASSERT_EQUAL_STRING("nihao\n", OutputSpy_GetLine(2));
    TEST_ASSERT_EQUAL_INT(strlen(str), OutputSpy_GetCharCount());
}

TEST(GeneratorOutputSpy, WriteMulityLine)
{
    char str[] = "\nhello world\nnihao\n";
    fwrite_spy(str, 1, strlen(str), NULL);
    int len = strlen(str);
    
    TEST_ASSERT_NOT_NULL(OutputSpy_GetLineArray());
    TEST_ASSERT_EQUAL_INT(3, OutputSpy_GetLineCount());
    TEST_ASSERT_EQUAL_STRING("\n", OutputSpy_GetLine(0));
    TEST_ASSERT_EQUAL_STRING("hello world\n", OutputSpy_GetLine(1));
    TEST_ASSERT_EQUAL_STRING("nihao\n", OutputSpy_GetLine(2));
    TEST_ASSERT_EQUAL_INT(len, OutputSpy_GetCharCount());

    char append1[] = "I'm here\nAnd you?";
    fwrite_spy(append1, 1, strlen(append1), NULL);
    len += strlen(append1);

    TEST_ASSERT_EQUAL_INT(5, OutputSpy_GetLineCount());
    TEST_ASSERT_EQUAL_STRING("I'm here\n", OutputSpy_GetLine(3));
    TEST_ASSERT_EQUAL_STRING("And you?", OutputSpy_GetLine(4));
    TEST_ASSERT_EQUAL_INT(len, OutputSpy_GetCharCount());

    char append2[] = " I'm fine.";
    fwrite_spy(append2, 1, strlen(append2), NULL);
    len += strlen(append2);

    TEST_ASSERT_EQUAL_INT(5, OutputSpy_GetLineCount());
    TEST_ASSERT_EQUAL_STRING("And you? I'm fine.", OutputSpy_GetLine(4));
    TEST_ASSERT_EQUAL_INT(len, OutputSpy_GetCharCount());

    char append3[] = "\nHow are you?";
    fwrite_spy(append3, 1, strlen(append3), NULL);
    len += strlen(append3);

    TEST_ASSERT_EQUAL_INT(6, OutputSpy_GetLineCount());
    TEST_ASSERT_EQUAL_STRING("And you? I'm fine.\n", OutputSpy_GetLine(4));
    TEST_ASSERT_EQUAL_STRING("How are you?", OutputSpy_GetLine(5));
    TEST_ASSERT_EQUAL_INT(len, OutputSpy_GetCharCount());
}

TEST_GROUP_RUNNER(GeneratorOutputSpy)
{
    RUN_TEST_CASE(GeneratorOutputSpy, Create);
    RUN_TEST_CASE(GeneratorOutputSpy, Destory);
    RUN_TEST_CASE(GeneratorOutputSpy, WriteNULL);
    RUN_TEST_CASE(GeneratorOutputSpy, WriteOneLine);
    RUN_TEST_CASE(GeneratorOutputSpy, WriteMulityLine);
}
