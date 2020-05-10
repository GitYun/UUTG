/**
 * \file strplus.h
 * \author vEmagic (admin@vemagic.com)
 * \brief String Helper Header
 * \version 0.0.1
 * \date 2020-04-25
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef STR_PLUS_H
#define STR_PLUS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

#define SL_ErrorMsg(format, ...)  SL_StringFormatPrint(stderr, (format), __VA_ARGS__)

/**
 * \brief 删除str字串中出现在trim字串中的字符
 * 
 * \param str 源字符串
 * \param trim 字符集合
 * \return char* 删除trim字串中字符后的str字串
 */
char* SL_StringTrim(char *str, char *trim);

/**
 * \brief 删除str字符串中出现的trimS字符串
 * 
 * \param str 源字符串
 * \param trimS 删除字符串
 * \return char* 删除trimS字串后的str字串
 */
char* SL_StringTrimString(char *str, char *trimS);

/**
 * \brief 删除str字串开头的空白字符
 * 
 * \param str 源字符串
 * \return char* 删除开头空白字符后的str字符串
 */
char* SL_StringTrimLeadWhitespace(char *str);

/**
 * \brief 用replace字串替换str字串中的target字串
 * 
 * \param str 源字符串
 * \param target 替换的目标字符串
 * \param replace 替换字符串
 * \return char* 处理后的str字串
 */
char* SL_StringReplace(char *str, char *target, const char *replace);

/**
 * \brief 在str字串后追加append字串
 * 
 * \param str 源字符串
 * \param ... 字符串列表, 以NULL为最后参数
 * \return char* 成功，则返回新字符串; 否正, 返回NULL
 * \note 当不使用新字符串后, 使用free释放内存
 */
char* SL_StringAppend(char *str, ...);

/**
 * \brief 在str字串开头添加字符串
 * 
 * \param str 源字符串
 * \param ... 字符串列表, 以NULL为最后参数
 * \return char* 成功，则返回新字符串; 否正, 返回NULL
 * \note 当不使用新字符串后, 使用free释放内存
 */
char* SL_StringAddHead(const char *str, ...);

/**
 * \brief 截取str中sidx位置到eidx位置的字符串
 * 
 * \param str 源字符串
 * \param sidx 开始索引
 * \param eidx 结束索引
 * \return char* 成功，则返回子字符串; 否则, 返回NULL
 * \note 当不使用子字符串后, 使用free释放内存
 */
char *SL_StringSubstring(const char *str, int sidx, int eidx);

/**
 * \brief 检查str字串是否以ends字串中任意字符结尾
 * 
 * \param str 源字符串
 * \param ends 字符集合
 * \return int str字串结尾字符出现在ends字符集合中的位置
 * \note 返回为负数, 则表示str结尾字符没有出现在ends中
 */
int SL_StringWithEnds(char *str, char *ends);

/**
 * \brief 检查str字串是否以ends字串中任意字符开头
 * 
 * \param str 源字符串
 * \param starts 字符集合 
 * \return int str字串开始字符出现在starts字符集合中的位置
 * \note 返回为负数, 则表示str开始字符没有出现在starts中
 */
int SL_StringWithStart(char *str, char *starts);

/**
 * \brief 检查str字串是否为目录路径
 * 
 * \param str 源字符串
 * \return true str是目录路径
 * \return false str不是目录路径
 */
bool SL_StringIsDirPath(const char *str);

/**
 * \brief 检查str字串是否为文件路径
 * 
 * \param str 源字符串
 * \return true str是文件路径
 * \return false str不是文件路径
 */
bool SL_StringIsFilePath(const char *str);

/**
 * \brief 以delim字符串中的字符分割str1, str2字符串, 
 *        比较每个分割字符串, 直到不同的分割字符串为止
 * 
 * \param str1 字符串1
 * \param str2 字符串2
 * \param delim 分割符集合
 * \return int 成功，则返回第几个分割字符串不同; 否则, 返回-1
 */
int SL_StringDelimDiff(char *str1, char *str2, const char *delim);

/**
 * \brief 获取str在delimS到delimE分割符之间的字符串
 * 
 * \param str 源字符串
 * \param delimS 左分割符
 * \param delimE 右分割符
 * \return char** 成功, 则返回两分隔符之间的字符串数组; 否则, 返回NULL
 * \note 不使用返回字符串数组时, 使用SL_StringArrayFree释放内存
 */
char** SL_StringDelimSubstring(const char *str, const char *delimS, const char *delimE);

/**
 * \brief 获取str在delimE到delimS分割符之间的字符串
 * 
 * \param str 源字符串
 * \param delimS 右分割符
 * \param delimE 左分割符
 * \return char** 成功, 则返回两分隔符之间的字符串数组; 否则, 返回NULL
 * \note 不使用返回字符串数组时, 使用SL_StringArrayFree释放内存
 */
char** SL_StringDelimSubstringReverse(const char *str, const char *delimS, const char *delimE);

/**
 * \brief 存储str字串到array字符串数组中
 * 
 * \param str 源字符串
 * \param array 字符串数组指针
 * \param count 字串串数组元素数量
 * \return char** 成功, 返回字符串数组; 否则, 返回NULL
 * \note 不使用字符串数据时, 使用SL_StringArrayFree释放内存
 */
char** SL_StringArrayLog(const char *str, char ***array, int *count);

/**
 * \brief 释放字符串数组内存
 * 
 * \param array 字符串数组指针
 * \param count 数量
 * \return true 内存释放成功
 * \return false 内存释放失败
 */
bool SL_StringArrayFree(char ***array, int *count);

/**
 * \brief 输出字符串数组到文件指针fp指向的文件
 * 
 * \param fp 文件指针
 * \param array 字符串数组
 * \param count 字符串数组元素的个数
 */
void SL_StringArrayPrint(FILE *fp, char **array, int count);

/**
 * \brief 格式化输出字符串到文件指针fp指向的文件
 * 
 * \param fp 文件指针
 * \param format 格式化字符串，参考printf函数
 * \param ... 字符串序列
 */
void SL_StringFormatPrint(FILE *fp, char *format, ...);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // STR_PLUS_H
