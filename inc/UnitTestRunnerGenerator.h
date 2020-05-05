/**
 * \file TestRunnerGenerator.h
 * \author vEmagic (admin@vemagic.com)
 * \brief 
 * \version 0.0.1
 * \date 2020-04-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef UNIT_TEST_RUNNER_GENERATOR_H
#define UNIT_TEST_RUNNER_GENERATOR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _test_group_
{
    char *group; // test group name
    char **name; // test name array
    int nameCount;
} tTestGroup;

/**
 * \fn void PrintGroupMessage(struct _test_group_ **groupArray, int count)
 * \brief 打印测试组信息到标准输出
 * \param [in|out] groupArray 测试组信息数组
 * \param [in] count 测试组的数量
 * \return None
 */
void PrintGroupMessage(struct _test_group_ **groupArray, int count);

/**
 * \fn int RunnerGenerator_GetTestGroupAndTestname(char **test, int count, struct _test_group_ **groupArray)
 * \brief 提取测试组信息
 * \param [in] test 函数名数组指针
 * \param [in] count 函数数量
 * \param [in|out] group 测试组信息数组指针
 * \return 测试组数量
 */
int RunnerGenerator_GetTestGroupAndTestname(char **test, int count, struct _test_group_ **groupArray);

/**
 * \fn void RunnerGenerator_TestGroupAndTestnameDestory(struct _test_group_ **groupArray, int groupCount)
 * \brief 释放测试数组信息所占内存
 * \param [in] groupArray 测试组信息数组指针
 * \param [in] groupCount 测试组数量
 * \return None
 */
void RunnerGenerator_TestGroupAndTestnameDestory(struct _test_group_ **groupArray, int groupCount);

/**
 * \fn void RunnerGenerator_GeneratorRunnerFile(struct _test_group_ **groupArray, int groupCount, const char *fileName)
 * \brief 生成测试运行容器源文件
 * \param [in] groupArray 测试组信息数组指针
 * \param [in] groupCount 测试组数量
 * \param [in] fileName 生成的源文件名（指定文件后缀，不需路径信息）
 * \return None
 */
void RunnerGenerator_GeneratorRunnerFile(struct _test_group_ **groupArray, int groupCount, const char *fileName);

/**
 * \fn void RunnerGenerator_GeneratorMainFile(struct _test_group_ **groupArray, int groupCount, char *path, char *fileName)
 * \brief 生成测试main文件
 * \param [in] groupArray 测试组信息数组指针
 * \param [in] groupCount 测试组数量
 * \param [in] path 保留
 * \param [in] fileName 生成的源文件名（指定文件后缀，不需路径信息）
 * \return None
 */
void RunnerGenerator_GeneratorMainFile(struct _test_group_ **groupArray, int groupCount, char *path, char *fileName);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // UNIT_TEST_RUNNER_GENERATOR_H
