// Copyright (c) 2020 Abhishek Mishra
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef SRC_CLI_TABLE_H_
#define SRC_CLI_TABLE_H_

#include "cli_common.h"
#include <stdlib.h>

#ifdef __cplusplus  
extern "C" {
#endif

typedef struct cli_table_t {
	size_t num_rows;
	size_t num_cols;
	char** header;
	char*** values;
} cli_table;

MODULE_API int create_cli_table(cli_table** table, size_t num_rows, size_t num_cols);

MODULE_API void free_cli_table(cli_table* table);

MODULE_API int cli_table_set_header(cli_table* table, size_t col_id, char* name);

MODULE_API int cli_table_set_row_val(cli_table* table, size_t row_id, size_t col_id,
		char* value);

MODULE_API int cli_table_get_header(char** name, cli_table* table, size_t col_id);

MODULE_API int cli_table_get_row_val(char** value, cli_table* table, size_t row_id,
	size_t col_id);

#ifdef __cplusplus 
}
#endif

#endif /* SRC_CLI_TABLE_H_ */
