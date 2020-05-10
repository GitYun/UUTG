/**
 * \file strplus.c
 * \author vEmagic (admin@vemagic.com)
 * \brief String Helper Base On the string.h
 * \version 0.0.1
 * \date 2020-04-25
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "strplus.h"

static bool isStringVaild(int n, ...);

// 删除str字串中出现在trim字串中的字符
char* SL_StringTrim(char *str, char *trim)
{
    if (!isStringVaild(2, str, trim))
    {
        return NULL;
    }

    char *pos = str;
    while ((pos = strpbrk(pos, trim)) != NULL)
    {
        memmove(pos, pos+1, strlen(pos+1) + 1);
    }

    return str;
}

// 删除str字符串中出现的trimS字符串
char* SL_StringTrimString(char *str, char *trimS)
{
    if (!isStringVaild(2, str, trimS))
    {
        return NULL;
    }

    int length = strlen(trimS);
    char *substr = str;
    while ((substr = strstr(substr, trimS)) != NULL)
    {
        memmove(substr, substr + length, strlen(substr + length) + 1);
    }
    
    return str;
}

// 删除str字串开头的空白字符
char* SL_StringTrimLeadWhitespace(char *str)
{
    if (!isStringVaild(1, str))
        return NULL;

    int spnsz = strspn(str, " \t\r\n\v\f");

    memmove(str, str + spnsz, strlen(str + spnsz));
    return str;
}

// 用replace字串替换str字串中的target字串
char* SL_StringReplace(char *str, char *target, const char *replace)
{
    if (!isStringVaild(3, str, target, replace))
    {
        return str;
    }

    int len = strlen(target);
    int tmplen = strlen(replace);

    char *pos = NULL;
    while ((pos = strstr(str, target)) != NULL)
    {
        for (int i = 0; i < len; ++i)
            pos[i] = replace[i % tmplen];
    }

    return str;
}

// 在str字串后追加append字串
char* SL_StringAppend(char *str, ...)
{
    if (!isStringVaild(1, str))
    {
        return NULL;
    }

    va_list va;
    va_start(va, str);

    int length = strlen(str) + 1;
    char *dst = strdup(str);

    char *append = NULL;
    while ((append = va_arg(va, char *)) != NULL)
    {
        length += strlen(append);
        dst = realloc(dst, length * sizeof(char));
        strncat(dst, append, strlen(append));
    }

    va_end(va);
    return dst;
}

// 在str字串开头添加字符串
char* SL_StringAddHead(const char *str, ...)
{
    if (!isStringVaild(1, str))
    {
        return NULL;
    }

    va_list va;
    va_start(va, str);

    int length = 1;
    char *dst = calloc(1, sizeof(char));
    char *append = NULL;
    while ((append = va_arg(va, char *)) != NULL)
    {
        length += strlen(append);
        dst = realloc(dst, length * sizeof(char));
        strncat(dst, append, strlen(append));
    }

    length += strlen(str);
    dst = realloc(dst, length * sizeof(char));
    strncat(dst, str, strlen(str));
    va_end(va);
    return dst;
}

// 截取str中sidx位置到eidx位置的字符串
char* SL_StringSubstring(const char *str, int sidx, int eidx)
{
    if (!isStringVaild(1, str) || sidx > eidx)
    {
        return NULL;
    }

    int length = eidx - sidx + 1;
    char *dst = calloc(length + 1, sizeof(char));
    if (!isStringVaild(1, dst))
    {
        return NULL;
    }

    strncpy(dst, str + sidx, length);
    return dst;
}

// 检查str字串是否以ends字串中任意字符结尾
int SL_StringWithEnds(char *str, char *ends)
{
    if (!isStringVaild(2, str, ends))
    {
        return -1;
    }

    char ch = str[strlen(str) - 1];
    return strchr(ends, ch) - ends;
}

// 检查str字串是否以ends字串中任意字符开头
int SL_StringWithStart(char *str, char *starts)
{
    if (!isStringVaild(2, str, starts))
    {
        return -1;
    }

    char ch = *str;
    return strchr(starts, ch) - starts;
}

// 检查str字串是否为目录路径
bool SL_StringIsDirPath(const char *str)
{
    if (!isStringVaild(1, str))
    {
        return false;
    }

    struct stat st;
    int ret = stat(str, &st);

    return ret == 0 && S_ISDIR(st.st_mode);
}

// 检查str字串是否为文件路径
bool SL_StringIsFilePath(const char *str)
{
    if (!isStringVaild(1, str))
    {
        return false;
    }

    struct stat st;
    int ret = stat(str, &st);
    return ret == 0 && S_ISREG(st.st_mode);
}

// 以delim字符串中的字符分割str1, str2字符串, 
// 比较每个分割字符串, 直到不同的分割字符串为止
int SL_StringDelimDiff(char *str1, char *str2, const char *delim)
{
    if (!isStringVaild(3, str1, str2, delim))
    {
        return -1;
    }

    char *dup1 = strdup(str1);
    char *dup2 = strdup(str2);

    if (!isStringVaild(2, dup1, dup2))
    {
        free(dup1);
        free(dup2);        
        return -1;
    }

    int ret = -1;
    char *cmp1 = strtok(dup1, delim);
    char *cmp2 = strtok(dup2, delim);

    while(cmp1 && cmp2)
    {
        ++ret;
        if (strcmp(cmp1, cmp2))
            break;
        cmp1 = strtok(dup1, NULL);
        cmp2 = strtok(dup2, NULL);
    }

    if (cmp1 == NULL || cmp2 == NULL)
    {
        ret = -1;
    }

    free(dup1);
    free(dup2);
    return ret;
}

// 获取str在delimS到delimE分割符之间的字符串
char** SL_StringDelimSubstring(const char *str, const char *delimS, const char *delimE)
{
    if (!isStringVaild(3, str, delimS, delimE))
    {
        return NULL;
    }

    int count = 1;
    char **array = (char **)calloc(1, sizeof(char *));
    if (!isStringVaild(1, array))
    {
        return NULL;
    }

    char *pos = (char *)str;
    while ((pos = strpbrk(pos, delimS)) > str)
    {
        ++pos; // 后移一字符不怕, 最多到'\0'
        char *delim = strpbrk(pos, delimE);
        if (delim && --delim > pos)
        {
            array = (char **)realloc(array, ++count * sizeof(char *));
            array[count - 2] = SL_StringSubstring(pos, 0, delim - pos);
            pos = delim + 1;
        }
    }

    array[count - 1] = NULL;
    return array;    
}

// 获取str在delimE到delimS分割符之间的字符串
char** SL_StringDelimSubstringReverse(const char *str, const char *delimS, const char *delimE)
{
    if (!isStringVaild(3, str, delimS, delimE))
    {
        return NULL;
    }

    int count = 1;
    char **array = (char **)calloc(1, sizeof(char *));
    if (!isStringVaild(1, array))
    {
        return NULL;
    }

    char *pos = (char *)str;
    char *delim = (char *)str;
    while ((pos = strpbrk(pos, delimS)) > str)
    {
        --pos; // 前移一字符不怕, 最多到str首字符
        char *tmp = delim = strpbrk(delim, delimE);
        if (delim && ++delim > pos)
            delim = (char *)str;

        if (delim)
        {
            array = (char **)realloc(array, ++count * sizeof(char *));
            array[count - 2] = SL_StringSubstring(delim, 0, pos - delim);
            pos++;
            delim = pos;
        }

        if (tmp == NULL)
            break;
    }

    array[count - 1] = NULL;
    return array;
}

// 存储str字串到array字符串数组中
char** SL_StringArrayLog(const char *str, char ***array, int *count)
{
    if (!isStringVaild(2, str, array) || *count < 0)
    {
        return NULL;
    }

    if (*array == NULL)
    {
        *count = 1;
        *array = (char **)calloc(2, sizeof(char *));
    }
    else
    {
        ++*count;
        *array = (char **)realloc(*array, (*count + 1) * sizeof(char *));
    }

    if (*array != NULL)
    {
        int idx = *count + 1;
        (*array)[idx - 2] = strdup(str);
        (*array)[idx - 1] = NULL;
    }
    return *array;
}

// 释放字符串数组内存
bool SL_StringArrayFree(char ***array, int *count)
{
    if (!isStringVaild(2, array, *array))
        return false;
    
    char **item = *array;
    int idx = 0;
    // while (item[idx] != NULL && ++idx);
    char *tmp = item[0];
    while (tmp != NULL)
    {
        ++idx;
        tmp = item[idx];
    }

    while (--idx >= 0)
    {
        if (item[idx])
            free(item[idx]);
    }

    free(*array);
    *array = NULL;
    *count = 0;
    return true;
}

// 输出字符串数组到文件指针fp指向的文件
void SL_StringArrayPrint(FILE *fp, char **array, int count)
{
    if (array == NULL || count < 1)
        return;

    fprintf(fp, "\n\nString Count: %d\n", count);
    for (int idx = 0; idx < count; ++idx)
    {
        if (array[idx])
        {
            fprintf(fp, "%s\n", array[idx]);
        }
    }
}

// 格式化输出字符串到文件指针fp指向的文件
void SL_StringFormatPrint(FILE *fp, char *format, ...)
{
    va_list va = NULL;
    va_start(va, format);

    if (va == NULL || fp == NULL || format == NULL)
        return;

    fprintf(fp, format, va);
    va_end(va);
}

static bool isStringVaild(int n, ...)
{
    bool ret = true;
    va_list va;
    va_start(va, n);

    for (int i = 0; i < n; ++i)
    {
        char *str = va_arg(va, char *);
        ret = (str != NULL);
        if (ret == false)
            break;
    }

    va_end(va);
    return ret;
}
