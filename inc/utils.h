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
 * \brief Get the Dirent Info object
 * 
 * \param dirPath 
 * \param filenames 
 * \param count 
 * \param keyword 
 * \return true 
 * \return false 
 */
bool GetDirentInfo(const char *dirPath, char ***filenames, int *count, const char *keyword);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // UTILS_H
