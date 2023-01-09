// Copyright (c) 2020 Abhishek Mishra
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef SRC_ZCLK_COMMON_H_
#define SRC_ZCLK_COMMON_H_

#include <stdlib.h>

#define MODULE_API_EXPORTS
#ifdef _WIN32
#  ifdef MODULE_API_EXPORTS
#    define MODULE_API __declspec(dllexport)
#  else
#    define MODULE_API __declspec(dllimport)
#  endif
#else
#  define MODULE_API
#endif

#ifdef __cplusplus  
extern "C" {
#endif

/**
 * Clone a string for usage in internal apis.
 * User of api can then get rid of their copy.
 * (obviously wasteful, use with caution)
 *
 * \param from string to clone from
 * \return cloned string, NULL if there is an error.
 */
MODULE_API char* zclk_str_clone(const char* from);

#ifdef __cplusplus 
}
#endif

#endif /* SRC_ZCLK_COMMON_H_ */
