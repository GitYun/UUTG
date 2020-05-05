#include "unity_fixture.h"

static int EnableSystem(void);
//-------------------------------------------------------------------------------------------

TEST_GROUP(Startup);

TEST_SETUP(Startup)
{
}

TEST_TEAR_DOWN(Startup)
{
}

TEST(Startup, EnableSys)
{
    TEST_ASSERT_TRUE(1 == EnableSystem());
}

//------------------------------------------------------------------------------------------

static int EnableSystem(void)
{
    return 1;
}
