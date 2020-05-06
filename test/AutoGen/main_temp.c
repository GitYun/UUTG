#include "unity_fixture.h"

static void RunTestAll(void)
{
	RUN_TEST_GROUP(CCodeParse_Test);
	RUN_TEST_GROUP(GeneratorOutputPrivate_Test);
	RUN_TEST_GROUP(stringHelper_Test);
	RUN_TEST_GROUP(UnityTestFuncName_Test);
	RUN_TEST_GROUP(UnitTestGenerator_Test);
	RUN_TEST_GROUP(utils_Test);
	RUN_TEST_GROUP(UnitTestRunnerGenerator_Test);
	RUN_TEST_GROUP(mpc_Test);
}

int main(int argc, char *argv[])
{
	return UnityMain(argc, argv, RunTestAll);
}
