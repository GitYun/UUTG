/**
 * \file uutg.h
 * \author vEmagic (admin@vemagic.com)
 * \brief Unity Unit Test Generator Header
 * \version 0.0.1
 * \date 2020-04-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef UUTG_H
#define UUTG_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _test_group_
{
    char *group; // test group name
    char **name; // test name array
    int nameCount;
} tTestGroup;

void UnitTestGenerator(char *inputPath, char *outputPath, char *suffix);

void GetFuncsDeclare(const char *filename, char ***ppsFuncs, int *count);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // UUTG_H
