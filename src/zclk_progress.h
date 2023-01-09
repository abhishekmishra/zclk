// Copyright (c) 2020 Abhishek Mishra
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef SRC_ZCLK_PROGRESS_H_
#define SRC_ZCLK_PROGRESS_H_

#include "zclk_common.h"
#include <coll_arraylist.h>

#ifdef __cplusplus  
extern "C" {
#endif

#define ZCLK_PROGRESS_DEFAULT_BAR "#"
#define ZCLK_PROGRESS_DEFAULT_BEFORE "["
#define ZCLK_PROGRESS_DEFAULT_AFTER "]"

typedef struct zclk_progress_t {
	char* name;
	char* message;
	int length;
	char* bar;
	char* before;
	char* after;
	char* extra;
	double current;
	double total;
} zclk_progress;

MODULE_API int create_zclk_progress(zclk_progress** progress, char* name, int length,
		double total);

MODULE_API void free_zclk_progress(zclk_progress* progress);

MODULE_API void show_progress(zclk_progress* progress);

typedef struct zclk_multi_progress_t {
	int old_count;
	arraylist* progress_ls;
} zclk_multi_progress;

MODULE_API int create_zclk_multi_progress(zclk_multi_progress** multi_progress);

MODULE_API void free_zclk_multi_progress(zclk_multi_progress* multi_progress);

#ifdef __cplusplus 
}
#endif

#endif /* SRC_ZCLK_PROGRESS_H_ */
