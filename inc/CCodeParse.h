/**
 * \file CCodeParse.h
 * \author vEmagic (admin@vemagic.com)
 * \brief 
 * \version 0.0.1
 * \date 2020-04-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef CCODEPARSE_H
#define CCODEPARSE_H

#include <stdbool.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \fn void CCP_GetCwd(char ** cwd)
 * \brief 获取当前工作路径
 * \param [in|out] cwd 指向当前工作目录路径字符串
 * \return None
 */
void CCP_GetCwd(char ** cwd);

/**
 * \fn void CCP_SetCwd(char *cwd)
 * \brief 设置当前工作路径
 * \param [in] cwd 工作路径字符串
 * \return None
 */
void CCP_SetCwd(char *cwd);

/**
 * \brief 检查文件路径字符串的有效性
 * 
 * \param path 路径字符串
 * \return true 路径有效
 * \return false 路径无效
 */
bool CCP_IsFilePathVaild(const char *path);

/**
 * \fn void CCP_PrintfMessage(char **messageAddr, int count)
 * \brief 打印信息到标准输出
 * \param [in] messageAddr 字符串数组
 * \param [int] count 字符串数量
 * \return None
 */
void CCP_PrintfMessage(char **messageAddr, int count);

/**
 * \fn char* CCP_StringSubtract(char *src, const char *subtract)
 * \brief 删除src字符串中包含在subtract的字符
 * \param [in|out] src 源字符串
 * \param [in] subtract 字符合集
 * \return 返回删除subtract字符合集后的src
 */
char* CCP_StringSubtract(char *src, const char *subtract);


/**
 * \brief 合并两个给定路径字符串
 * 
 * \param dirPath 路径字符串
 * \param add 追加路径字符串
 * \return char* 合并后的路径字符串
 * \note 返回不为NULL时, 在不在使用返回字符串后, 使用free释放内存
 */
char* CCP_CombinePath(char *dirPath, char *add);

/**
 * \fn void CCP_GetDirFileInfo(char *dirPath, char ***filenameArray, int *count, char *file, char *suffix)
 * \brief 递归查找给定目录路径下的绝对文件名, 缓存并排序
 * \param [in] dirPath 目录路径字符串
 * \param [in|out] filenameArray 绝对文件名字符串的缓存数组
 * \param [in|out] count 指向文件数量缓存对象
 * \param [in] file 查找文件名
 * \param [in] suffix 文件名后缀
 * \return None
 */
void CCP_GetDirFileInfo(char *dirPath, char ***filenameArray, int *count, char *file, char *suffix);

/**
 * \fn void CCP_GetFuncNameFormSource(char **fileArray, int fileCount, char ***test, int *count)
 * \brief 从源文件中提取函数名
 * \param [in|out] fileArray 文件名数组（绝对路径）
 * \param [in] fileCount 文件数量
 * \param [in|out] functions 指向函数名数组对象
 * \param [in|out] count 提取的函数数量
 * \return None
 */
void CCP_GetFuncNameFormSource(char **fileArray, int fileCount, char ***functions, int *count);

/**
 * \fn void CCP_Free(char ***filenameArray, int *count)
 * \brief 释放filenameArray指向字符串数组的内存
 * \param [in|out] filenameArray 指向字符串数组对象
 * \param [in|out] count 指向文件数量缓存对象
 * \return None
 */
void CCP_Free(char ***filenameArray, int *count);

#if __cplusplus
}
#endif // __cplusplus

#endif // CCODEPARSE_H

