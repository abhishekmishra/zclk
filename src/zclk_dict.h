// Copyright (c) 2020 Abhishek Mishra
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef SRC_CLD_DICT_H_
#define SRC_CLD_DICT_H_

#include "zclk_common.h"
#include <coll_arraylist.h>

#ifdef __cplusplus  
extern "C" {
#endif

typedef struct zclk_dict_t {
	arraylist* keys;
	arraylist* vals;
} zclk_dict;

MODULE_API int create_zclk_dict(zclk_dict** dict);

MODULE_API void free_zclk_dict(zclk_dict* dict);

MODULE_API int zclk_dict_put(zclk_dict* dict, char* key, char* value);

MODULE_API int zclk_dict_remove(zclk_dict* dict, char* key);

MODULE_API int zclk_dict_get(zclk_dict* dict, char* key, char** value);

MODULE_API int zclk_dict_keys(zclk_dict* dict, char** keys);

#define zclk_dict_foreach(dict, key, value) \
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
