/**
 * \file CCodeeParse.c
 * \author vEmagic (admin@vemagic.com)
 * \brief 
 * \version 0.0.1
 * \date 2020-04-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include "CCodeParse.h"

static int checkPathDiff(char *cwd, char *path);
static int stringCompare(const void * p1, const void * p2);
static bool isDirPathWithEndSlash(char *dirPath);
static char* addSlashToDirPath(char *dirPath);
static void skipFunctionBody(FILE *fp, const char *filename, int *lineno);

// 获取当前工作路径
void CCP_GetCwd(char ** cwd)
{
    *cwd = getcwd(NULL, 0);
}

// 设置当前工作路径
void CCP_SetCwd(char *cwd)
{
    chdir(cwd);
}

// 检查文件路径字符串的有效性
bool CCP_IsFilePathVaild(const char *path)
{
    return path != NULL;
}

// 打印信息到标准输出
void CCP_PrintfMessage(char **messageAddr, int count)
{
    if (messageAddr && count > 0)
    {
        fprintf(stdout, "\n");
        fprintf(stdout, "\nMessage count: %d\n", count);
        for (int idx = 0; idx < count; ++idx)
        {
            if (messageAddr[idx])
                fprintf(stdout, "%s\n", messageAddr[idx]);
        }
    }
}

// 删除src字符串中包含在subtract的字符
char* CCP_StringSubtract(char *src, const char *subtract)
{
    if (src == NULL || subtract == NULL)
    {
        return src;
    }

    int len = strlen(src);
    char *pos = NULL;
    while (len > 0 && (pos = strpbrk(src, subtract)) != NULL)
    {
        memmove(pos, pos+1, strlen(pos+1)+1);
        src[len] = '\0';
        --len;
    }
    return src;
}

// 检测两个给定路径字符串的的差异出现在第几个路径分隔符后
// 返回0, 则两个路径相同
static int checkPathDiff(char *cwd, char *path)
{
    assert(cwd != NULL && path != NULL);

    int tokCount = 0;
    char *cwdTmp = (char *)calloc(strlen(cwd)+1, sizeof(char));
    char *pathTmp = (char *)calloc(strlen(path) + 1, sizeof(char));

    if (cwdTmp && pathTmp)
    {   
        cwdTmp = strncpy(cwdTmp, cwd, strlen(cwd) + 1);
        pathTmp = strncpy(pathTmp, path, strlen(path) + 1);

        char *cwdTok = strtok(cwdTmp, "\\/");
        char *pathTok = strtok(pathTmp, "\\/");
        int cwdLen = strlen(cwdTok); 
        int pathLen = strlen(pathTok);
        while (cwdLen == pathLen && !strcmp(cwdTok, pathTok))
        {
            ++tokCount;
            cwdTok = strtok(NULL, "\\/");
            pathTok = strtok(NULL, "\\/");
            cwdLen += strlen(cwdTok);
            pathLen += strlen(pathTok);
        }
    }

    if (cwdTmp)
        free(cwdTmp);

    if (pathTmp)
        free(pathTmp);

    return tokCount;
}

// 检测给定路径字符串是否以路径分隔符结尾
static bool isDirPathWithEndSlash(char *dirPath)
{
    char dirPathEndChar = dirPath[strlen(dirPath) - 1];
    return dirPathEndChar == '\\' || dirPathEndChar == '/';
}

// 使路径字符串以路径分隔符结尾
// 返回不为NULL时, 在不在使用返回字符串后, 使用free释放内存
static char* addSlashToDirPath(char *dirPath)
{
    assert(dirPath != NULL);

    int dirPathWithSlashLength = strlen(dirPath) + 2;
    char *dirPathWithSlash = (char *)calloc(dirPathWithSlashLength, sizeof(char));
    if (dirPathWithSlash != NULL)
    {
        strncpy(dirPathWithSlash, dirPath, dirPathWithSlashLength - 1);
        dirPathWithSlash[dirPathWithSlashLength - 1] = '\0';

        if (!isDirPathWithEndSlash(dirPath))
        {
            dirPathWithSlash[dirPathWithSlashLength - 2] = '\\';
        }
    }

    return dirPathWithSlash;
}

// 合并两个给定路径字符串
// 返回不为NULL时, 在不在使用返回字符串后, 使用free释放内存
char* CCP_CombinePath(char *dirPath, char *add)
{
    char *path = addSlashToDirPath(dirPath);

    int len = strlen(path) + strlen(add) + 1;
    char *combinePath = NULL;
    if (NULL != (combinePath = (char *)calloc(len, sizeof(char))))
    {
        strncpy(combinePath, path, strlen(path));
        strncat(combinePath, add, strlen(add));
        combinePath[len - 1] = '\0';
    }

    if (path)
        free(path);

    return combinePath;
}

// 跳过源文件的函数体
static void skipFunctionBody(FILE *fp, const char *filename, int *lineno)
{
#define LINE_BUFFER_SIZE 1024

    int lineSize = LINE_BUFFER_SIZE;
    char *line = (char *)calloc(lineSize, sizeof(char));

    int bodyStartCount = 0, bodyEndCount = 0;
    while (fgets(line, lineSize, fp) != NULL)
    {
        ++*lineno;
        if (strstr(line, "{"))
        {
            ++bodyStartCount;
        }

        if (strstr(line, "}"))
        {
            ++bodyEndCount;
            if (bodyStartCount == bodyEndCount)
                break;
        }
    }
    free(line);
    if (bodyStartCount != bodyEndCount)
        fprintf(stdout, "\n%s, %d line: Function body miss the character \'{\' or \'}\'\n", filename, *lineno);
}

// 字符大小比较
static int stringCompare(const void * p1, const void * p2)
{
    char * e1 = *(char **)p1;
    char * e2 = *(char **)p2;
    return strcasecmp(e1, e2);
}

// 递归查找给定目录路径下的绝对文件名, 缓存并排序
void CCP_GetDirFileInfo(char *dirPath, char ***filenameArray, int *count, char *file, char *suffix)
{
    if (!filenameArray)
        return;

    if (*count < 0)
        *count = 0;

    int ret = 0;
    int fc = *count;
    char *currentDirPath = getcwd(NULL, 0);
    char *dirFullPath = currentDirPath;
    if (dirPath && strcmp(dirPath, currentDirPath) != 0)
    {
        if (checkPathDiff(currentDirPath, dirPath) == 0)
            dirFullPath = CCP_CombinePath(currentDirPath, dirPath);

        ret = chdir(dirFullPath);
    }

    DIR *dir = opendir(dirFullPath);
    if (ret != 0 || dir == NULL)
    {
        if (dir == NULL)
            printf("\n\"%s\" isn't a directory!\n", dirFullPath);
        else
            printf("\nFailed to change the current directory to \"%s\".\n", dirFullPath);
    }
    else
    {
        struct dirent *dirent;
        struct stat st;

        while ((dirent = readdir(dir)) != NULL) // 获取目录信息
        {
            // 忽略当前目录与父目录
            if (!strcmp(dirent->d_name, ".") || !strcmp(dirent->d_name, ".."))
                continue;

            int ret = stat(&dirent->d_name[0], &st); //获取文件信息
            if (ret != 0)
                continue;

            char *subPath = CCP_CombinePath(dirFullPath, dirent->d_name);
            if (subPath == NULL)
                continue;

            if (S_ISREG(st.st_mode)) // 是普通文件
            {
                // 判断文件名与后缀是否符合要求
                bool bFile = false, bSuffix = false;
                if (file)
                    bFile = (NULL != strstr(dirent->d_name, file));
                
                if (suffix && strlen(suffix) > 1)
                {
                    bSuffix = (dirent->d_name[dirent->d_namlen - 2] == suffix[0]);
                    bSuffix &= (dirent->d_name[dirent->d_namlen - 1] == suffix[1]);
                }

                if ((!bFile && !bSuffix) || (!suffix && !bFile) || (!file && !bSuffix))
                {
                    free(subPath);
                    continue;
                }

                ++*count;
                if ((*filenameArray = (char **)realloc((*filenameArray), *count * sizeof(char *))) != NULL)
                {
                    (*filenameArray)[*count - 1] = subPath;
                }
            }
            else if (S_ISDIR(st.st_mode)) // 是目录, 则递归
            {
                CCP_GetDirFileInfo(subPath, filenameArray, count, file, suffix);
                free(subPath);
            }
            else
            {
                free(subPath);
            }
        }
        closedir(dir);

        if (*count > fc && *filenameArray != NULL && ***filenameArray != '\0')
        {
            qsort(*filenameArray, *count, sizeof(char *), stringCompare);
        }
    }

    if (dirFullPath != dirPath && dirFullPath != currentDirPath)
        free(dirFullPath);

    chdir(currentDirPath);
    free(currentDirPath);
}

// 从源文件中提取函数名
void CCP_GetFuncNameFormSource(char **fileArray, int fileCount, char ***functions, int *count)
{
#define LINE_BUFFER_SIZE 1024
    int lineSize = LINE_BUFFER_SIZE;
    char *line = (char *)calloc(lineSize, sizeof(char));
    int lineno = 0;

    for (int idx = 0; idx < fileCount; ++idx)
    {
        FILE *fp = fopen(fileArray[idx], "r");

        if (fp == NULL)
            continue;

        // 设置文件流以换行为依据的无缓冲IO
        setvbuf(fp, NULL, _IOLBF, 0);
        int pos = EOF;
        while (0 == feof(fp) && line != NULL)
        {
            pos = ftell(fp); // Store current file pointer
            if (EOF != pos && NULL != fgets(line, lineSize, fp))
            {
                int len = strlen(line); // Line feed character '\n' is last character
                if ((len + 1) == lineSize && line[len-1] != '\n') // line buffer not enough
                {
                    lineSize += (LINE_BUFFER_SIZE >> 1);
                    line = (char *)realloc(line, lineSize * sizeof(char));
                    if (0 != fseek(fp, pos, SEEK_SET)) // 移动文件流的读写位置到本次读写前的记录位置
                        break;

                    continue;
                }
                ++lineno;

                // Subtract sapce and othre character before the `T' and after the `)'
                char *target = strstr(line, "TEST(");
                if (target != NULL)
                {
                    char *target_s = strstr(line, "IGNORE_TEST(");
                    char *target_e = strrchr(line, ')');

                    if (target_s != NULL)
                        target = target_s;

                    if (target_e != NULL)
                        len = (target_e - target) + 2; // Has a `\0'(NUL) placeholder
                    
                    target[len - 1] = '\0';
                    ++ *count;
                    if (NULL != (*functions = (char **)realloc(*functions, *count * sizeof(char *))))
                    {
                        char ** tmp = &(*functions)[*count - 1];
                        if (NULL != (*tmp = (char *)calloc(len, sizeof(char))))
                        {
                            strncpy(*tmp, target, len);
                            CCP_StringSubtract(*tmp, " \t");
                        }
                    }

                    skipFunctionBody(fp, fileArray[idx], &lineno);
                }
            }
        }
        fclose(fp);
    }

    if (line != NULL)
        free(line);
}

// 释放filenameArray指向字符串数组的内存
void CCP_Free(char ***filenameArray, int *count)
{
    if (filenameArray == NULL || *filenameArray == NULL)
        return;

    char **file = *filenameArray;
    int fileCnt = *count;
    for (int idx = 0; idx < fileCnt; ++idx)
    {
        if (file[idx] != NULL)
        {
            free(file[idx]);
            -- *count;
        }
    }
    free(file);
    *filenameArray = NULL;
}