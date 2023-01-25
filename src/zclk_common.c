/**
 * Copyright (c) 2020 Abhishek Mishra
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include "zclk_common.h"
#include <string.h>

char* zclk_str_clone(const char* from) {
	char* to = NULL;
	if ((from != NULL)) {
		to = (char*) malloc((strlen(from) + 1) * sizeof(char));
		if (to != NULL) {
			strcpy(to, from);
		}
	}
	return to;
}
