// Copyright (c) 2020 Abhishek Mishra
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef SRC_ZCLK_TABLE_H_
#define SRC_ZCLK_TABLE_H_

#include "zclk_common.h"
#include <stdlib.h>

#ifdef __cplusplus  
extern "C" {
#endif

typedef struct zclk_table_t {
	size_t num_rows;
	size_t num_cols;
	char** header;
	char*** values;
} zclk_table;

MODULE_API int create_zclk_table(zclk_table** table, size_t num_rows, size_t num_cols);

MODULE_API void free_zclk_table(zclk_table* table);

MODULE_API int zclk_table_set_header(zclk_table* table, size_t col_id, char* name);

MODULE_API int zclk_table_set_row_val(zclk_table* table, size_t row_id, size_t col_id,
		char* value);

MODULE_API int zclk_table_get_header(char** name, zclk_table* table, size_t col_id);

MODULE_API int zclk_table_get_row_val(char** value, zclk_table* table, size_t row_id,
	size_t col_id);

#ifdef __cplusplus 
}
#endif

#endif /* SRC_ZCLK_TABLE_H_ */
