#include "unity_fixture.h"

static void RunTestAll(void)
{
	RUN_TEST_GROUP(Control);
	RUN_TEST_GROUP(DIDO);
	RUN_TEST_GROUP(Startup);
}

int main(int argc, char *argv[])
{
	return UnityMain(argc, argv, RunTestAll);
}
