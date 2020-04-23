/**
 * Copyright (c) 2020 Abhishek Mishra
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include "cli_common.h"

char* str_clone(const char* from) {
	char* to = NULL;
	if ((from != NULL)) {
		to = (char*) malloc((strlen(from) + 1) * sizeof(char));
		if (to != NULL) {
			strcpy(to, from);
		}
	}
	return to;
}