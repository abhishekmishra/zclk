// Copyright (c) 2020 Abhishek Mishra
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef SRC_CLD_DICT_H_
#define SRC_CLD_DICT_H_

#include "cli_common.h"
#include <arraylist.h>

#ifdef __cplusplus  
extern "C" {
#endif

typedef struct cli_dict_t {
	arraylist* keys;
	arraylist* vals;
} cli_dict;

MODULE_API int create_cli_dict(cli_dict** dict);

MODULE_API void free_cli_dict(cli_dict* dict);

MODULE_API int cli_dict_put(cli_dict* dict, char* key, char* value);

MODULE_API int cli_dict_remove(cli_dict* dict, char* key);

MODULE_API int cli_dict_get(cli_dict* dict, char* key, char** value);

MODULE_API int cli_dict_keys(cli_dict* dict, char** keys);

#define cli_dict_foreach(dict, key, value) \
	char* key; \
	char* value; \
	size_t i; \
	size_t len = arraylist_length(dict->keys); \
	for(i = 0, \
			key = (char*)arraylist_get(dict->keys, i), \
			value = (char*)arraylist_get(dict->vals, i); \
			i < len; \
			i++, \
			key = (char*)arraylist_get(dict->keys, i), \
			value = (char*)arraylist_get(dict->vals, i))

#ifdef __cplusplus 
}
#endif

#endif /* SRC_CLD_DICT_H_ */
