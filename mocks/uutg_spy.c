/**
 * \file uutg_spy.c
 * \author vEmagic (admin@vemagic.com)
 * \brief Unity Unit Test Generator Spyer
 * \version 0.0.1
 * \date 2020-04-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unity_fixture_malloc_overrides.h"
#include "uutg_spy.h"

static char lineend = '\0';
static char **lineMessage = NULL;
static int lineCount = 0;
static int charCount = 0;

// 查找str字符串中存在多少个ch字符
static int strnchr(char *str, int ch)
{
    int count = 0;
    char *p = str;
    while ((p = strchr(p, ch)) != NULL)
    {
        ++count;
        ++p;
    }
    return count;
}

// 查找字符ch出现在字符串str的哪些位置
static int *strchrpos(char *str, int ch)
{
    int *array = NULL;
    char *pos = str;
    for (int count = 1; (pos = strchr(pos, ch)) != NULL; ++count)
    {
        array = (int *)realloc(array, count * sizeof(int));
        array[count - 1] = pos - str;
        pos += 1;
    }
    return array;
}

// 追加字符串append的前count个字符
static void fwrite_append(const char *append, int count)
{
    if (lineCount < 1)
        return;
    
    int len = 0, total = 0;
    char **line = &lineMessage[lineCount - 1];
    if (*line != NULL)
        len = strlen(*line);
    
    total = len + count; 
    *line = (char *)realloc(*line, (total + 1) * sizeof(char));
    (*line)[len] = '\0';

    strncat(*line, append, count);
    charCount += count;
}

// 将字符串写入新行
static void fwrite_newline(const char *str, int count)
{
    ++lineCount;
    lineMessage = (char **)realloc(lineMessage, lineCount * sizeof(char *));
    lineMessage[lineCount - 1] = NULL;
    fwrite_append(str, count);
}

// 写入字符串
static void fwrite_line(const char *str, int count)
{
    if (lineend == '\n')
        fwrite_newline(str, count);
    else
        fwrite_append(str, count);
}

// 监视输入的字符串
size_t fwrite_spy(char *src, size_t size, size_t count, char *dest)
{
    if (!src || count == 0)
        return 0;
    
    char *pos = src;
    char *prevpos = src;
    while ((pos = strchr(pos, '\n')) != NULL)
    {
        pos += 1;
        fwrite_line(prevpos, pos - prevpos);
        prevpos = pos;
        lineend = '\n';
    }

    if (prevpos == src || prevpos[0] != '\0')
    {
        fwrite_line(prevpos, strlen(prevpos));
    }

    lineend = src[count - 1];

    return pos - src;
}

// 释放内存
static void OutputSpy_FreeMemory(void *mem)
{
    if (mem)
        free(mem);
}

// 释放内存数组
static void OutputSpy_FreeMemoryArray(int memArray, int count)
{
    if (memArray == 0 || count < 1)
        return;

    char **array = (char **)memArray;
    for (int i = 0; i < count; ++i)
    {
        char *ele = array[i];
        OutputSpy_FreeMemory(ele);
    }
}

// 输出监视器私有数据设置
static void OutputSpy_Set(void *pointer, int lineNumber, int charNumber)
{
    lineMessage = (char **)pointer;
    lineCount = lineNumber;
    charCount = charNumber;
    lineend = '\n';
}

// 创建输出监视器
void OutputSpy_Create(void)
{
    char **tmp = (char **)calloc(1, sizeof(char *));
    OutputSpy_Set(tmp, 0, 0);
}

// 销毁输出监视器
void OutputSpy_Destory(void)
{
    OutputSpy_FreeMemoryArray((int)lineMessage, lineCount);
    OutputSpy_FreeMemory(lineMessage);
    OutputSpy_Set(NULL, 0, 0);
}

// 获取输出监视器消息数组对象
char **OutputSpy_GetLineArray(void)
{
    return lineMessage;
}

// 获取输出监视器中的指定行消息
char *OutputSpy_GetLine(int lineno)
{
    if (!lineMessage)
        return NULL;
    if (lineno >= lineCount)
        return NULL;
    return lineMessage[lineno];
}

// 获取输出监视器消息行的数量
int OutputSpy_GetLineCount(void)
{
    return lineCount;
}

// 获取输出监视器消息字符数量
int OutputSpy_GetCharCount(void)
{
    return charCount;
}
