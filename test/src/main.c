#include "unity_fixture.h"

static void RunTestsAll(void)
{
    // RUN_TEST_GROUP(GeneratorOutputSpy);
    // RUN_TEST_GROUP(RunnerGenerator);
    RUN_TEST_GROUP(UnitTestGenerator);
}

int main(int argc, char* argv[])
{
    return UnityMain(argc, argv, RunTestsAll);
}
