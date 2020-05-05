/**
 * \file MocksGenerator.h
 * \author vEmagic (admin@vemagic.com)
 * \brief 
 * \version 0.0.1
 * \date 2020-04-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef UNITY_TEST_GENERATOR_H
#define UNITY_TEST_GENERATOR_H

#ifdef __cplusplus
extern "C" {
#endif

char* readFile(FILE* fp);
void UnitTestGenerator(char *inputPath, char *outputPath, char *suffix);
void GetFuncsDeclare(const char *filename, char ***ppsFuncs, int *count);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // UNITY_TEST_GENERATOR_H
