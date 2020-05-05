#include "unity_fixture.h"
#include "GeneratorOutputPrivate.h"

TEST_GROUP(GeneratorOutput);

#define STRING(str) #str
#define FILE_NAME STRING(template.c)

TEST_SETUP(GeneratorOutput)
{
    outputCFileCreate(FILE_NAME);
}

TEST_TEAR_DOWN(GeneratorOutput)
{
    outputCFileDestory(FILE_NAME);
}

TEST(GeneratorOutput, FileCreateThenDestory)
{
    //TEST_ASSERT_EQUAL_STRING(FILE_NAME, GetOutputCFileNameSpy());
}
