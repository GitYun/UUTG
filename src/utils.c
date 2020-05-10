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
#include "strplus.h"
#include "utils.h"

static int stringCompare(const void * p1, const void * p2);

// 递归搜索给定目录下与keyword匹配到的文件名, 通过字母顺序排序并存储
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
                char *tmp = SL_StringAppend((char *)dirPath, dirent->d_name, NULL);
                SL_StringArrayLog(tmp, filenames, count);
                free(tmp);
            }
        }
        else if (S_ISDIR(st.st_mode)) // 是目录, 则递归
        {
            char* subDirPath = SL_StringAppend((char *)dirPath, dirent->d_name, NULL);
            GetDirentInfo(subDirPath, filenames, count, keyword);
            free(subDirPath);
        }
    }
    closedir(dir);

    qsort(*filenames, *count, sizeof(char *), stringCompare);
    chdir(cwd);
    free(cwd);
    return true;
}

// 字符大小比较
static int stringCompare(const void * p1, const void * p2)
{
    char * e1 = (char *)p1;
    char * e2 = (char *)p2;
    return strcasecmp(e1, e2);
}
