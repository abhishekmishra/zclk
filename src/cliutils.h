// Copyright (c) 2020 Abhishek Mishra
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef SRC_CLD_COMMAND_H_
#define SRC_CLD_COMMAND_H_

#include <arraylist.h>
#include <lua.h>
#include <lauxlib.h>

#define CLD_OPTION_HELP_LONG "help"
#define CLD_OPTION_HELP_SHORT "h"
#define CLD_OPTION_HELP_DESC "Print help for command."

typedef enum
{
	CLD_COMMAND_IS_RUNNING = -1,
	CLD_COMMAND_SUCCESS = 0,
	CLD_COMMAND_ERR_UNKNOWN = 1,
	CLD_COMMAND_ERR_ALLOC_FAILED = 2,
	CLD_COMMAND_ERR_COMMAND_NOT_FOUND = 3,
	CLD_COMMAND_ERR_OPTION_NOT_FOUND = 4,
	CLD_COMMAND_ERR_ARG_NOT_FOUND = 5,
	CLD_COMMAND_ERR_EXTRA_ARGS_FOUND = 6
} cld_cmd_err;

typedef enum
{
	CLD_TYPE_BOOLEAN = 0,
	CLD_TYPE_INT = 1,
	CLD_TYPE_DOUBLE = 2,
	CLD_TYPE_STRING = 3,
	CLD_TYPE_FLAG = 4
} cld_type;

typedef enum
{
	CLD_RESULT_STRING = 0,
	CLD_RESULT_TABLE = 1,
	CLD_RESULT_DICT = 2,
	CLD_RESULT_PROGRESS = 3
} cld_result_type;

//TODO add defaults
typedef struct cld_val_t
{
	cld_type type;
	int bool_value;
	int int_value;
	double dbl_value;
	char* str_value;
} cld_val;

int cld_val_to_lua(lua_State *L, cld_val* val);

cld_val* create_cld_val(cld_type type, int bool_val, int int_val, double dbl_val, char* str);

#define CLD_VAL_FLAG(v) create_cld_val(CLD_TYPE_FLAG, v, 0, 0, NULL)
#define CLD_VAL_BOOLEAN(v) create_cld_val(CLD_TYPE_BOOLEAN, v, 0, 0, NULL)
#define CLD_VAL_INT(v) create_cld_val(CLD_TYPE_INT, 0, v, 0, NULL)
#define CLD_VAL_DOUBLE(v) create_cld_val(CLD_TYPE_DOUBLE, 0, 0, v, NULL)
#define CLD_VAL_STRING(v) create_cld_val(CLD_TYPE_STRING, 0, 0, 0, v)

typedef struct cld_option_t
{
	char* name;
	char* short_name;
	cld_val* val;
	cld_val* default_val;
	char* description;
} cld_option;

int cld_option_to_lua(lua_State *L, cld_option* option);
void arraylist_cld_option_to_lua(lua_State *L, int index, void *data);

typedef struct cld_argument_t
{
	char* name;
	cld_val* val;
	cld_val* default_val;
	char* description;
	int optional;
} cld_argument;

int cld_argument_to_lua(lua_State *L, cld_argument* arg);
void arraylist_cld_argument_to_lua(lua_State *L, int index, void *data);

void cld_fill_options_in_list(arraylist* optlist, cld_option* options[]);

typedef cld_cmd_err(*cld_command_output_handler)(cld_cmd_err result_flag,
	cld_result_type result_type, void* result);

typedef cld_cmd_err(*cld_command_handler)(void* handler_args,
	arraylist* options, arraylist* args,
	cld_command_output_handler success_handler,
	cld_command_output_handler error_handler);

typedef struct cld_command_t
{
	char* name;
	char* short_name;
	char* description;
	arraylist* sub_commands;
	arraylist* options;
	arraylist* args;
	cld_command_handler handler;
} cld_command;

/**
 * Create a new value object of given type.
 *
 * \param val object to create
 * \param type
 * \return error code
 */
cld_cmd_err make_cld_val(cld_val** val, cld_type type);

/**
 * Free the created value
 */
void free_cld_val(cld_val* val);

/**
 * Reset values to system defaults.
 */
void clear_cld_val(cld_val* val);

/**
 * Copy values from 'from' to 'to'.
 * Can be used to reset to defaults.
 *
 * \param to val to set
 * \param from val to read from
 */
void copy_cld_val(cld_val* to, cld_val* from);

/**
 * Parse the input and read the value of the type of the val object.
 * (Should not be called when the values is a flag.)
 * The value should be set as soon as the argument/option is seen
 *
 * \param val object whose value will be set
 * \param input string input
 * \return error code
 */
cld_cmd_err parse_cld_val(cld_val* val, char* input);

/**
 * Create a new option given a name and type.
 *
 * \param option object to create
 * \param name
 * \param short_name
 * \param value
 * \param default value
 * \param description
 * \return error code
 */
cld_cmd_err make_option(cld_option** option, char* name, char* short_name,
    cld_val* val, cld_val* default_val, char* description);

cld_option* create_option(char* name, char* short_name, cld_val* val, cld_val* default_val, char* desc);

/**
 * Free resources used by option
 */
void free_option(cld_option* option);

cld_option* get_option_by_name(arraylist* options, char* name);

/**
 * Create a new argument given a name and type.
 *
 * \param argument object to create
 * \param name
 * \param type
 * \param description
 * \return error code
 */
cld_cmd_err make_argument(cld_argument** arg, char* name, cld_val* val, cld_val* default_val, char* desc);
cld_argument* create_argument(char* name, cld_val* val, cld_val* default_val, char* desc);

/**
 * Free resources used by argument
 */
void free_argument(cld_argument* arg);

/**
 * Create a new command with the given name and handler
 * Options and sub-commands need to be added after creation.
 * The sub-commands, arguments and options lists will be initialized,
 * so one just needs to add items using the arraylist add function.
 *
 * \param command obj to be created
 * \param name
 * \param short_name
 * \param description
 * \param handler function ptr to handler
 * \return error code
 */
cld_cmd_err make_command(cld_command** command, char* name, char* short_name,
	char* description, cld_command_handler handler);

cld_command* create_command(char* name, char* short_name,
    char* description, cld_command_handler handler);

/**
 * Free a command object
 */
void free_command(cld_command* command);

/**
* Get help for a command
* \param cmds_to_exec the list of commands and subcommands parsed
* \return string with command help
*/
char* get_help_for_command(arraylist* cmds_to_exec);

/**
 * Run the help command for all commands or single command
 *
 * \param commands the list of commands registered (this is a list of cld_command*)
 * \param handler_args an args value to be passed to the command handler
 * \param argc the number of tokens in the line
 * \param argv args as an array of strings
 * \param success_handler handle success results
 * \param error_handler handler error results
 */
cld_cmd_err help_cmd_handler(arraylist* commands, void* handler_args,
	int argc, char** argv, cld_command_output_handler success_handler,
	cld_command_output_handler error_handler);

/**
 * Get the help string for the arg_commands from the registered commands list.
 * \param help_str the help string to return
 * \param commands is the configured list of commands
 * \param arg_commands is a list of string
 * \return error code
 */
cld_cmd_err get_help_for(char** help_str, arraylist* commands,
	arraylist* arg_commands);

/**
 * Execute a single line containing one top-level command.
 * All output is written to stdout, all errors to stderr
 *
 * \param commands the list of commands registered (this is a list of cld_command*)
 * \param handler_args an args value to be passed to the command handler
 * \param argc the number of tokens in the line
 * \param argv args as an array of strings
 * \param success_handler handle success results
 * \param error_handler handler error results
 */
cld_cmd_err exec_command(arraylist* commands, void* handler_args,
	int argc, char** argv, cld_command_output_handler success_handler,
	cld_command_output_handler error_handler);

#endif /* SRC_CLD_COMMAND_H_ */
