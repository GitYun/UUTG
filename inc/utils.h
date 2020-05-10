/**
 * \file utils.h
 * \author vEmagic (admin@vemagic.com)
 * \brief 
 * \version 0.0.1
 * \date 2020-05-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

/**
 * \brief 递归搜索给定目录下与keyword匹配到的文件名, 通过字母顺序排序并存储
 * 
 * \param dirPath 目录路径
 * \param filenames 文件名缓存器
 * \param count 文件数量
 * \param keyword 文件名匹配关键字
 * \return true 搜索成功
 * \return false 搜索失败
 */
bool GetDirentInfo(const char *dirPath, char ***filenames, int *count, const char *keyword);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // UTILS_H
