/**
 * \file GeneratorOutputPrivate.c
 * \author vEmagic (admin@vemagic.com)
 * \brief 
 * \version 0.0.1
 * \date 2020-04-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "GeneratorOutputPrivate.h"
#include "GeneratorOutputSpy.h"

static char *lineFeed = "\n";
static FILE *fp = NULL;

// 创建输出文件
void outputCFileCreate(const char *filename)
{

}

// 销毁创建文件所用资源
void outputCFileDestory(const char *filename)
{
    
}

// 输出生成信息到文件
void outputCFileGenerateMessage(FILE *fp, const char *msg)
{
    char timestr[100] = {' ', '*', ' ', '\0'};
    time_t ptime;
    time(&ptime);
    char *localtime = ctime(&ptime);
    strcat(timestr, localtime);

    fwrite("/*\n", sizeof(char), 3, fp);
    if (msg)
        fwrite(msg, sizeof(char), strlen(msg), fp);
    fwrite(timestr, sizeof(char), strlen(timestr), fp);
    fwrite(" */\n\n", sizeof(char), 5, fp);

    // char *generateMessage = "/* THE FILE IS GENERATED, DO NOT EDITE IT. */\n";
    // fwrite(generateMessage, sizeof(char), strlen(generateMessage), fp);
}

// 输出#include头文件到文件
void outputCFileHead(FILE *fp, char **includes, int count)
{
    assert(fp != NULL);
    assert(count > 0);
    assert(includes != NULL);

    for (int idx = 0; idx < count && includes[idx] != NULL; ++idx)
    {
        fwrite(includes[idx], sizeof(char), strlen(includes[idx]), fp);
        fwrite(lineFeed, sizeof(char), strlen(lineFeed), fp);
    }
    // fwrite(lineFeed, sizeof(char), strlen(lineFeed), fp);
}

// 输出函数实现到文件
void outputCFileFunctionStart(FILE *fp, char *funcNameAndleftBracket, char **args, int argc)
{
    fwrite(funcNameAndleftBracket, sizeof(char), strlen(funcNameAndleftBracket), fp);

    int idx = 0;
    for (; idx < argc && args[idx] != NULL; ++idx)
    {
        if (idx > 0)
        {
            char *comma = ", ";
            fwrite(comma, sizeof(char), strlen(comma), fp);
        }
        fwrite(args[idx], sizeof(char), strlen(args[idx]), fp);
    }

    // if (idx == 0)
    // {
    //     char * voidParam = "void";
    //     fwrite(voidParam, sizeof(char), strlen(voidParam), fp);
    // }

    if (args && argc > 0)
    {
        char *rightBracket = ")";
        fwrite(rightBracket, sizeof(char), strlen(rightBracket), fp);
    }
    char *funcImplStart = "\n{\n";
    fwrite(funcImplStart, sizeof(char), strlen(funcImplStart), fp);
}

// 输出函数调用到文件
void outputCFileFunctionCall(FILE *fp, char *funcNameAndleftBracket, char **args, int argc)
{
    fwrite(funcNameAndleftBracket, sizeof(char), strlen(funcNameAndleftBracket), fp);

    for (int idx = 0; idx < argc; ++idx)
    {
        char *comma = ", ";
        if (idx > 0)
            fwrite(comma, sizeof(char), strlen(comma), fp);

        fwrite(args[idx], sizeof(char), strlen(args[idx]), fp);
    }

    char *rightBracket = ");\n";
    fwrite(rightBracket, sizeof(char), strlen(rightBracket), fp);
}

// 输出函数实现结束到文件
void outputCFileFunctionEnd(FILE *fp, char *ends)
{
    fwrite(ends, sizeof(char), strlen(ends), fp);
    fwrite(lineFeed, sizeof(char), strlen(lineFeed), fp);
}

// 输出空行到为文件
void outputCFileBlankLine(FILE *fp)
{
    fwrite(lineFeed, sizeof(char), strlen(lineFeed), fp);
}