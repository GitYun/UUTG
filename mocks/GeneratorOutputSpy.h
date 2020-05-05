/**
 * \file GeneratorOutputSpy.h
 * \author vEmagic (admin@vemagic.com)
 * \brief 
 * \version 0.0.1
 * \date 2020-04-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef _GENERATOR_OUTPUT_H
#define _GENERATOR_OUTPUT_H

//#define fwrite_spy  fwrite

/**
 * \fn size_t fwrite_spy(char *src, size_t size, size_t count, char *dest)
 * \brief 监视输入的字符串
 * \param [in] src 监视字符串
 * \param [in] size 字符大小，总字节数为 size * count
 * \param [in] count 字符数量
 * \param [out] dest 保留
 * \return 写入的字符数
 */
size_t fwrite_spy(char *src, size_t size, size_t count, char *dest);

/**
 * \fn void OutputSpy_Create(void)
 * \brief 创建输出监视器
 * \param None
 * \return None
 */
void OutputSpy_Create(void);

/**
 * \fn void OutputSpy_Destory(void)
 * \brief 销毁输出监视器
 * \param None
 * \return None
 */
void OutputSpy_Destory(void);

/**
 * \fn char **OutputSpy_GetLineArray(void)
 * \brief 获取输出监视器消息数组对象
 * \param None
 * \return 消息数组对象
 */
char **OutputSpy_GetLineArray(void);

/**
 * \fn char *OutputSpy_GetLine(int lineno)
 * \brief 获取输出监视器中的指定行消息
 * \param None
 * \return 指定行消息
 */
char *OutputSpy_GetLine(int lineno);

/**
 * \fn int OutputSpy_GetLineCount(void)
 * \brief 获取输出监视器消息行的数量
 * \param None
 * \return 消息行数
 */
int OutputSpy_GetLineCount(void);

/**
 * \fn int OutputSpy_GetCharCount(void))
 * \brief 获取输出监视器消息字符数量
 * \param None
 * \return 字符数
 */
int OutputSpy_GetCharCount(void);

#endif