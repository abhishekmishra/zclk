// Copyright (c) 2020 Abhishek Mishra
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef SRC_CLI_PROGRESS_H_
#define SRC_CLI_PROGRESS_H_

#include "cli_common.h"
#include <arraylist.h>

#ifdef __cplusplus  
extern "C" {
#endif

#define CLI_PROGRESS_DEFAULT_BAR "#"
#define CLI_PROGRESS_DEFAULT_BEFORE "["
#define CLI_PROGRESS_DEFAULT_AFTER "]"

typedef struct cli_progress_t {
	char* name;
	char* message;
	int length;
	char* bar;
	char* before;
	char* after;
	char* extra;
	double current;
	double total;
} cli_progress;

MODULE_API int create_cli_progress(cli_progress** progress, char* name, int length,
		double total);

MODULE_API void free_cli_progress(cli_progress* progress);

MODULE_API void show_progress(cli_progress* progress);

typedef struct cli_multi_progress_t {
	int old_count;
	arraylist* progress_ls;
} cli_multi_progress;

MODULE_API int create_cli_multi_progress(cli_multi_progress** multi_progress);

MODULE_API void free_cli_multi_progress(cli_multi_progress* multi_progress);

#ifdef __cplusplus 
}
#endif

#endif /* SRC_CLI_PROGRESS_H_ */
