/**
 * Copyright (c) 2020 Abhishek Mishra
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdlib.h>
#include "cli_table.h"

int create_cli_table(cli_table** table, size_t num_rows, size_t num_cols) {
	(*table) = (cli_table*) calloc(1, sizeof(cli_table));
	if (!(*table)) {
		return 1;
	}
	(*table)->num_cols = num_cols;
	(*table)->num_rows = num_rows;
	(*table)->header = (char**) calloc(num_cols, sizeof(char*));
	for (int i = 0; i < num_cols; i++) {
		(*table)->header[i] = NULL;
	}
	(*table)->values = (char***) calloc(num_rows, sizeof(char**));
	for (int i = 0; i < num_rows; i++) {
		(*table)->values[i] = (char**) calloc(num_cols, sizeof(char*));
		for (int j = 0; j < num_cols; j++) {
			(*table)->values[i][j] = NULL;
		}
	}
	return 0;
}

void free_cli_table(cli_table* table) {
	for (size_t i = 0; i < table->num_cols; i++) {
		free(table->header[i]);
	}
	free(table->header);
	for (size_t i = 0; i < table->num_rows; i++) {
		for (size_t j = 0; j < table->num_cols; j++) {
			free(table->values[i][j]);
		}
		free(table->values[i]);
	}
	free(table->values);
	free(table);
}

int cli_table_set_header(cli_table* table, size_t col_id, char* name) {
	if (col_id >= 0 && col_id < table->num_cols) {
		table->header[col_id] = str_clone(name);
		return 0;
	} else {
		return -1;
	}
}

int cli_table_set_row_val(cli_table* table, size_t row_id, size_t col_id, char* value) {
	if (col_id >= 0 && col_id < table->num_cols && row_id >= 0
			&& row_id < table->num_rows) {
		table->values[row_id][col_id] = str_clone(value);
		return 0;
	} else {
		return -1;
	}
}

int cli_table_get_header(char** name, cli_table* table, size_t col_id) {
	if (col_id >= 0 && col_id < table->num_cols) {
		(*name) = table->header[col_id];
		return 0;
	}
	return -1;
}

int cli_table_get_row_val(char** value, cli_table* table, size_t row_id,
	size_t col_id) {
	if (col_id >= 0 && col_id < table->num_cols && row_id >= 0
			&& row_id < table->num_rows) {
		(*value) = table->values[row_id][col_id];
		return 0;
	}
	return -1;
}

