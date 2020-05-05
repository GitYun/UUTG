/**
 * \file UnityTestFuncName.h
 * \author vEmagic (admin@vemagic.com)
 * \brief 
 * \version 0.0.1
 * \date 2020-05-05
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef UNITYTESTFUNCNAME_H
#define UNITYTESTFUNCNAME_H

#ifdef __cplusplus
extern "C" {
#endif

#include "UnitTestRunnerGenerator.h"

int GetTestFuncDeclare(char **ppsFiles, int fileCount, char ***pppsFuncs, int *count, tTestGroup **ppsTestGroup);
int GetGroupAndTestName(char **ppsFuncs, int count, struct _test_group_ **ppsTestGroup);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // UNITYTESTFUNCNAME_H
