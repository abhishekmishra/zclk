/**
 * Copyright (c) 2020 Abhishek Mishra
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdlib.h>
#include "cli_dict.h"

int create_cli_dict(cli_dict** dict) {
	(*dict) = (cli_dict*)calloc(1, sizeof(cli_dict));
	if(!(*dict)) {
		return -1;
	}
	arraylist_new(&((*dict)->keys), &free);
	arraylist_new(&((*dict)->vals), &free);
	return 0;
}

void free_cli_dict(cli_dict* dict) {
	arraylist_free(dict->keys);
	arraylist_free(dict->vals);
	free(dict);
}

int cli_dict_put(cli_dict* dict, char* key, char* value) {
	arraylist_add(dict->keys, cli_str_clone(key));
	arraylist_add(dict->vals, cli_str_clone(value));
	return 0;
}

//TODO: Not implemented
int cli_dict_remove(cli_dict* dict, char* key) {
	return -1;
}

//TODO: Not implemented
int cli_dict_get(cli_dict* dict, char* key, char** value) {
	return -1;
}

//TODO: Not implemented
int cli_dict_keys(cli_dict* dict, char** keys) {
	return -1;
}

