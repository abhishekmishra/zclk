/**
 * Copyright (c) 2020 Abhishek Mishra
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdlib.h>
#include "zclk_dict.h"

int create_zclk_dict(zclk_dict** dict) {
	(*dict) = (zclk_dict*)calloc(1, sizeof(zclk_dict));
	if(!(*dict)) {
		return -1;
	}
	arraylist_new(&((*dict)->keys), &free);
	arraylist_new(&((*dict)->vals), &free);
	return 0;
}

void free_zclk_dict(zclk_dict* dict) {
	arraylist_free(dict->keys);
	arraylist_free(dict->vals);
	free(dict);
}

int zclk_dict_put(zclk_dict* dict, char* key, char* value) {
	arraylist_add(dict->keys, zclk_str_clone(key));
	arraylist_add(dict->vals, zclk_str_clone(value));
	return 0;
}

//TODO: Not implemented
int zclk_dict_remove(zclk_dict* dict, char* key) {
	return -1;
}

//TODO: Not implemented
int zclk_dict_get(zclk_dict* dict, char* key, char** value) {
	return -1;
}

//TODO: Not implemented
int zclk_dict_keys(zclk_dict* dict, char** keys) {
	return -1;
}

