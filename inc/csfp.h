/**
 * \file csfp.h
 * \author vEmagic (admin@vemagic.com)
 * \brief C Source File Printer Header
 * \version 0.0.1
 * \date 2020-04-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef CSFP_H
#define CSFP_H

#include <assert.h>

/**
 * \fn void outputCFileCreate(const char *filename)
 * \brief 创建输出文件
 * \param [in] filename
 * \return None
 */
void outputCFileCreate(const char *filename);

/**
 * \fn void outputCFileDestory(const char *filename)
 * \brief
 * \param [in] filename
 * \return None
 */
void outputCFileDestory(const char *filename);

/**
 * \fn void outputCFileGenerateMessage(FILE *fp)
 * \brief 销毁创建文件所用资源
 * \param [in] fp
 * \return None
 */
void outputCFileGenerateMessage(FILE *fp, const char *msg);

/**
 * \fn void outputCFileHead(FILE *fp, char **includes, int count)
 * \brief 输出#include头文件到文件
 * \param
 * \param
 * \return None
 */
void outputCFileHead(FILE *fp, char **includes, int count);

/**
 * \fn void outputCFileFunctionStart(FILE *fp, char *funcNameAndleftBracket, char **args, int argc)
 * \brief 输出函数实现到文件
 * \param
 * \param
 * \return None
 */
void outputCFileFunctionStart(FILE *fp, char *funcNameAndleftBracket, char **args, int argc);

/**
 * \fn void outputCFileFunctionCall(FILE *fp, char *funcNameAndleftBracket, char **args, int argc)
 * \brief 输出函数调用到文件
 * \param [in] fp
 * \param [in] funcName
 * \return None
 */
void outputCFileFunctionCall(FILE *fp, char *funcNameAndleftBracket, char **args, int argc);

/**
 * \fn void outputCFileFunctionEnd(FILE *fp, char *ends)
 * \brief 输出函数实现结束到文件
 * \param [in] fp
 * \param [in] ends
 * \return None
 */
void outputCFileFunctionEnd(FILE *fp, char *ends);

/**
 * \fn void outputCFileBlankLine(FILE *fp)
 * \brief 输出空行到为文件
 * \param [in] fp
 * \return None
 */
void outputCFileBlankLine(FILE *fp);

#endif // CSFP_H