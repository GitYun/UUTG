/**
 * \file utils.c
 * \author vEmagic (admin@vemagic.com)
 * \brief 
 * \version 0.0.1
 * \date 2020-05-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include "stringHelper.h"
#include "utils.h"

static int stringCompare(const void * p1, const void * p2);

// Get the Dirent Info object
bool GetDirentInfo(const char *dirPath, char ***filenames, 
                   int *count, const char *keyword)
{
    if (dirPath && !SL_StringIsDirPath(dirPath))
    {
        SL_ErrorMsg("\n\"%s\" %s\n", dirPath, "is not a vaild direntory path.");
        return false;
    }
    
    if (filenames == NULL || count == NULL)
        return false;
    
    if (keyword == NULL || strlen(keyword) == 0)
        return false;
    
    DIR *dir = opendir(dirPath);
    if (dir == NULL)
    {
        SL_ErrorMsg("\n%s '%s' %s\n", "Open the direntory", dirPath, "failed!");
        return false;
    }

    char *fullDirPath = NULL;
    if (SL_StringWithEnds((char *)dirPath, "\\/") < 0)
        fullDirPath = SL_StringAppend((char *)dirPath, "/", NULL);
    else
        fullDirPath = strdup(dirPath);

    struct dirent *dirent;
    struct stat st;

    char *cwd = getcwd(NULL, 0);
    chdir(dirPath);
    while ((dirent = readdir(dir)) != NULL) // 获取目录信息
    {
        // 忽略当前目录与父目录
        if (!strcmp(dirent->d_name, ".") || !strcmp(dirent->d_name, ".."))
            continue;

        int ret = stat(&dirent->d_name[0], &st); //获取文件信息
        if (ret != 0)
            continue;
        
        if (S_ISREG(st.st_mode)) // 是普通文件
        {
            if (strstr(dirent->d_name, keyword))
            {
                ++*count;
                if ((*filenames = realloc(*filenames, *count * sizeof(char *))) != NULL)
                {
                    (*filenames)[*count - 1] = SL_StringAppend(fullDirPath, dirent->d_name, NULL);
                }
            }
        }
        else if (S_ISDIR(st.st_mode)) // 是目录, 则递归
        {
            char* subDirPath = SL_StringAppend(fullDirPath, dirent->d_name, NULL);
            GetDirentInfo(subDirPath, filenames, count, keyword);
            free(subDirPath);
        }
    }
    closedir(dir);

    qsort(*filenames, *count, sizeof(char *), stringCompare);
    chdir(cwd);
    free(cwd);
    free(fullDirPath);
    return true;
}

// 字符大小比较
static int stringCompare(const void * p1, const void * p2)
{
    char * e1 = (char *)p1;
    char * e2 = (char *)p2;
    return strcasecmp(e1, e2);
}
