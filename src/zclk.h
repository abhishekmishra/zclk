// Copyright (c) 2020 Abhishek Mishra
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

/**
 * \file zclk.h
 * \brief Zclk main header file. Defines the Public API of zclk.
 */

#ifndef __ZCLK_H__
#define __ZCLK_H__

#include "cli_common.h"

#include <coll_arraylist.h>
#include <lua.h>
#include <lauxlib.h>
#include "cli_table.h"
#include "cli_dict.h"
#include "cli_progress.h"

#ifdef __cplusplus  
extern "C" {
#endif

/** Help option long name */
#define ZCLK_OPTION_HELP_LONG "help"
/** Help option short name */
#define ZCLK_OPTION_HELP_SHORT "h"
/** Help option description */
#define ZCLK_OPTION_HELP_DESC "Print help for command."

/**
 * @brief This enum defines the possible error codes 
 * 	generated by functions in the API.
 */
typedef enum
{
	ZCLK_COMMAND_IS_RUNNING = -1,
	ZCLK_COMMAND_SUCCESS = 0,
	ZCLK_COMMAND_ERR_UNKNOWN = 1,
	ZCLK_COMMAND_ERR_ALLOC_FAILED = 2,
	ZCLK_COMMAND_ERR_COMMAND_NOT_FOUND = 3,
	ZCLK_COMMAND_ERR_OPTION_NOT_FOUND = 4,
	ZCLK_COMMAND_ERR_ARG_NOT_FOUND = 5,
	ZCLK_COMMAND_ERR_EXTRA_ARGS_FOUND = 6
} zclk_cmd_err;

/**
 * @brief This enum defines the possible datatypes of 
 * 	argument or option values
 */
typedef enum
{
	ZCLK_TYPE_BOOLEAN = 0,
	ZCLK_TYPE_INT = 1,
	ZCLK_TYPE_DOUBLE = 2,
	ZCLK_TYPE_STRING = 3,
	ZCLK_TYPE_FLAG = 4
} zclk_type;

#define ZCLK_BOOL_TRUE 	1
#define ZCLK_BOOL_FALSE 0
#define ZCLK_FLAG_ON 	1
#define ZCLK_FLAG_OFF	0

/**
 * @brief This enum defines the possible types of result 
 * 	cli program might output.
 */
typedef enum
{
	ZCLK_RESULT_STRING = 0,
	ZCLK_RESULT_TABLE = 1,
	ZCLK_RESULT_DICT = 2,
	ZCLK_RESULT_PROGRESS = 3
} zclk_result_type;

//TODO add defaults
/**
 * @brief This struct holds the value of the argument or option.
 * 
 */
typedef struct zclk_val_t
{
	zclk_type type;			///< type of value
	union {
		int bool_value;		///< boolean value
		int int_value;		///< integer value
		double dbl_value;	///< double value
		char* str_value;	///< string value
	} data;					///< data of the value
} zclk_val;

MODULE_API int zclk_val_is_type(zclk_val *val, zclk_type type);

/**
 * @brief Check if given value is boolean
 * 
 * @param val value object
 * @return flag indicating if value is boolean
 */
#define zclk_val_is_bool(val)		zclk_val_is_type(val, ZCLK_TYPE_BOOLEAN)

/**
 * @brief Check if given value is int
 * 
 * @param val value object
 * @return flag indicating if value is int
 */
#define zclk_val_is_int(val)		zclk_val_is_type(val, ZCLK_TYPE_INT)

/**
 * @brief Check if given value is double
 * 
 * @param val value object
 * @return flag indicating if value is double
 */
#define zclk_val_is_double(val)		zclk_val_is_type(val, ZCLK_TYPE_DOUBLE)

/**
 * @brief Check if given value is string
 * 
 * @param val value object
 * @return flag indicating if value is string
 */
#define zclk_val_is_string(val)		zclk_val_is_type(val, ZCLK_TYPE_STRING)

/**
 * @brief Check if given value is flag
 * 
 * @param val value object
 * @return flag indicating if value is flag
 */
#define zclk_val_is_flag(val)		zclk_val_is_type(val, ZCLK_TYPE_FLAG)

/**
 * @brief get the boolean value
 * 
 * @param val value object
 * @return boolean value
 */
MODULE_API int zclk_val_get_bool(zclk_val *val);

/**
 * @brief get the int value
 * 
 * @param val value object
 * @return int value
 */
MODULE_API int zclk_val_get_int(zclk_val *val);

/**
 * @brief get the double value
 * 
 * @param val value object
 * @return double value
 */
MODULE_API double zclk_val_get_double(zclk_val *val);

/**
 * @brief get the string value
 * 
 * @param val value object
 * @return string value
 */
MODULE_API char* zclk_val_get_string(zclk_val *val);

/**
 * @brief get the flag value
 * 
 * @param val value object
 * @return flag value
 */
MODULE_API int zclk_val_get_flag(zclk_val *val);

/**
 * @brief set the boolean value
 * 
 * @param val value object
 * @param bval boolean value
 */
MODULE_API void zclk_val_set_bool(zclk_val *val, int bval);

/**
 * @brief set the int value
 * 
 * @param val value object
 * @param bval int value
 */
MODULE_API void zclk_val_set_int(zclk_val *val, int ival);

/**
 * @brief set the double value
 * 
 * @param val value object
 * @param bval double value
 */
MODULE_API void zclk_val_set_dobule(zclk_val *val, double dval);

/**
 * @brief set the string value
 * 
 * @param val value object
 * @param bval string value
 */
MODULE_API void zclk_val_set_string(zclk_val *val, char* sval);

/**
 * @brief set the flag value
 * 
 * @param val value object
 * @param bval flag value
 */
MODULE_API void zclk_val_set_flag(zclk_val *val, int fval);

/**
 * @brief Convert a cli value to its lua value
 * 
 * @param L Lua state
 * @param val value
 * @return int indicating error
 */
MODULE_API int zclk_val_to_lua(lua_State *L, zclk_val* val);

MODULE_API zclk_val* new_zclk_val_bool(int bool_val);
MODULE_API zclk_val* new_zclk_val_int(int int_val);
MODULE_API zclk_val* new_zclk_val_double(double double_val);
MODULE_API zclk_val* new_zclk_val_string(char* string_val);
MODULE_API zclk_val* new_zclk_val_flag(int flag_val);

/**
 * @brief Create a new flag cli value
 * 
 * @param v value
 */
#define zclk_flag(v) new_zclk_val_flag(v)

/**
 * @brief Create a new boolean cli value
 * 
 * @param v value
 */
#define zclk_boolean(v) new_zclk_val_bool(v)

/**
 * @brief Create a new integer cli value
 * 
 * @param v value
 */
#define zclk_int(v) new_zclk_val_int(v)

/**
 * @brief Create a new double cli value
 * 
 * @param v value
 */
#define zclk_double(v) new_zclk_val_double(v)

/**
 * @brief Create a new string cli value
 * 
 * @param v value
 */
#define zclk_string(v) new_zclk_val_string(v)

/**
 * @brief CLI Option Object
 */
typedef struct zclk_option_t
{
	char* name;				///< name of the option
	char* short_name;		///< short_name of the option
	zclk_val* val;			///< value of the option
	zclk_val* default_val;	///< default value of the option
	char* description;		///< textural description of the option
} zclk_option;

/**
 * @brief Convert the cli option to a lua value
 * 
 * @param L lua state
 * @param option option object
 * @return int error code
 */
MODULE_API int zclk_option_to_lua(lua_State *L, zclk_option* option);

/**
 * @brief Utility conversion function passed to arraylist to convert all entries to lua options
 * 
 * @param L lua state
 * @param index 
 * @param data must be a zclk_option*
 */
MODULE_API void arraylist_zclk_option_to_lua(lua_State *L, int index, void *data);

/**
 * @brief CLI Argument object
 */
typedef struct zclk_argument_t
{
	char* name;				///< name of the argument
	zclk_val* val;			///< value of the argument
	zclk_val* default_val;	///< default value of the argument
	char* description;		///< textual description
	int optional;			///< flag indicating if argument is optional
} zclk_argument;

/**
 * @brief Convert a cli argument to a lua object
 * 
 * @param L lua state
 * @param arg cli argument
 * @return int error code
 */
MODULE_API int zclk_argument_to_lua(lua_State *L, zclk_argument* arg);

/**
 * @brief Utility conversion function passed to arraylist to convert all entries to lua
 * 
 * @param L lua state
 * @param index
 * @param data must be a zclk_argument*
 */
MODULE_API void arraylist_zclk_argument_to_lua(lua_State *L, int index, void *data);

/**
 * @brief Fill the entries in the given option array into an arraylist
 * 
 * @param optlist arraylist to fill
 * @param options options array to use
 */
MODULE_API void zclk_fill_options_in_list(arraylist* optlist, zclk_option* options[]);

/**
 * @brief Defines a function to handle command output
 */
typedef zclk_cmd_err(*zclk_command_output_handler)(zclk_cmd_err result_flag,
	zclk_result_type result_type, void* result);

// declration to assist in defining command fn
struct zclk_command_t;

/**
 * @brief defines a function to run a command
 */
typedef zclk_cmd_err(*zclk_command_fn)(struct zclk_command_t* cmd,
										void* handler_args);

/**
 * @brief A CLI Command Ojbect
 */
typedef struct zclk_command_t
{
	char* name;						///< name of the command
	char* short_name;				///< short name of the command
	char* description;				///< textual description
	arraylist* sub_commands;		///< list of subcommands
	arraylist* options;				///< options list
	arraylist* args;				///< args list
	zclk_command_fn handler;		///< command handler function
	zclk_command_output_handler
		error_handler;				///< error handler for the command
	zclk_command_output_handler
		success_handler;			///< success handler for the command
} zclk_command;

/**
 * Create a new value object of given type.
 *
 * \param val object to create
 * \param type
 * \return error code
 */
MODULE_API zclk_cmd_err make_zclk_val(zclk_val** val, zclk_type type);

/**
 * Free the created value
 */
MODULE_API void free_zclk_val(zclk_val* val);

/**
 * Reset values to system defaults.
 */
MODULE_API void clear_zclk_val(zclk_val* val);

/**
 * Copy values from 'from' to 'to'.
 * Can be used to reset to defaults.
 *
 * \param to val to set
 * \param from val to read from
 */
MODULE_API void copy_zclk_val(zclk_val* to, zclk_val* from);

/**
 * Parse the input and read the value of the type of the val object.
 * (Should not be called when the values is a flag.)
 * The value should be set as soon as the argument/option is seen
 *
 * \param val object whose value will be set
 * \param input string input
 * \return error code
 */
MODULE_API zclk_cmd_err parse_zclk_val(zclk_val* val, char* input);

/**
 * (Internal Use) Create a new option given a name and type.
 *
 * \param option object to create
 * \param name
 * \param short_name
 * \param val
 * \param default_val
 * \param description
 * \return error code
 */
MODULE_API zclk_cmd_err make_option(zclk_option** option, char* name, 
	char* short_name, zclk_val* val, zclk_val* default_val, char* description);

/**
 * @brief (Internal Use) Create an option object
 * 
 * NOTE:
 * For most usecases use the type specific option creation functions
 * called \c new_zclk_option_<type>() . Here type can be one of bool, int
 * flag, double or string.
 * 
 * @param name name of the option
 * @param short_name short name
 * @param val value object
 * @param default_val default value object
 * @param desc description
 * @return MODULE_API* 
 */
MODULE_API zclk_option* new_zclk_option(char* name, char* short_name, 
	zclk_val* val, zclk_val* default_val, char* desc);

MODULE_API zclk_option *new_zclk_option_bool(char *name, char *short_name, 
	int val, int default_val, char *desc);
MODULE_API zclk_option *new_zclk_option_int(char *name, char *short_name, 
	int val, int default_val, char *desc);
MODULE_API zclk_option *new_zclk_option_double(char *name, char *short_name, 
	double val, double default_val, char *desc);
MODULE_API zclk_option *new_zclk_option_string(char *name, char *short_name, 
	char* val, char* default_val, char *desc);
MODULE_API zclk_option *new_zclk_option_flag(char *name, char *short_name, 
	int val, int default_val, char *desc);

MODULE_API char *zclk_option_get_name(zclk_option *opt);
MODULE_API char *zclk_option_get_short_name(zclk_option *opt);
MODULE_API char *zclk_option_get_desc(zclk_option *opt);

MODULE_API int zclk_option_get_val_bool(zclk_option *opt);
MODULE_API int zclk_option_get_val_int(zclk_option *opt);
MODULE_API double zclk_option_get_val_double(zclk_option *opt);
MODULE_API char* zclk_option_get_val_string(zclk_option *opt);
MODULE_API int zclk_option_get_val_flag(zclk_option *opt);

MODULE_API int zclk_option_get_default_val_bool(zclk_option *opt);
MODULE_API int zclk_option_get_default_val_int(zclk_option *opt);
MODULE_API double zclk_option_get_default_val_double(zclk_option *opt);
MODULE_API char* zclk_option_get_default_val_string(zclk_option *opt);
MODULE_API int zclk_option_get_default_val_flag(zclk_option *opt);

/**
 * Free resources used by option
 *
 * @param option to free
 */
MODULE_API void free_option(zclk_option* option);

/**
 * @brief Get the option by name object
 * 
 * @param options options list
 * @param name name of option to retrieve
 * @return zclk_option* option with the given name
 */
MODULE_API zclk_option* get_option_by_name(arraylist* options, char* name);

/**
 * (Internal Use) Create a new argument given a name and type.
 *
 * NOTE:
 * For most usecases use the type specific argument creation functions
 * called \c new_zclk_argument_<type>() . Here type can be one of bool, int
 * flag, double or string.
 * 
 * \param arg object to create
 * \param name
 * \param val
 * \param default_val
 * \param desc
 * \return error code
 */
MODULE_API zclk_cmd_err make_argument(zclk_argument** arg, char* name, 
	zclk_val* val, zclk_val* default_val, char* desc);

/**
 * @brief (Internal use) Create an argument object
 * 
 * NOTE:
 * For most usecases use the type specific argument creation functions
 * called \c new_zclk_argument_<type>() . Here type can be one of bool, int
 * flag, double or string.
 * 
 * @param name 
 * @param val 
 * @param default_val 
 * @param desc 
 * @return MODULE_API* 
 */
MODULE_API zclk_argument* new_zclk_argument(char* name, zclk_val* val, 
	zclk_val* default_val, char* desc);

MODULE_API zclk_argument *new_zclk_argument_bool(char *name, 
	int val, int default_val, char *desc);
MODULE_API zclk_argument *new_zclk_argument_int(char *name, 
	int val, int default_val, char *desc);
MODULE_API zclk_argument *new_zclk_argument_double(char *name, 
	double val, double default_val, char *desc);
MODULE_API zclk_argument *new_zclk_argument_string(char *name, 
	char* val, char* default_val, char *desc);
MODULE_API zclk_argument *new_zclk_argument_flag(char *name, 
	int val, int default_val, char *desc);

MODULE_API char *zclk_argument_get_name(zclk_argument *opt);
MODULE_API char *zclk_argument_get_desc(zclk_argument *opt);

MODULE_API int zclk_argument_get_val_bool(zclk_argument *opt);
MODULE_API int zclk_argument_get_val_int(zclk_argument *opt);
MODULE_API double zclk_argument_get_val_double(zclk_argument *opt);
MODULE_API char* zclk_argument_get_val_string(zclk_argument *opt);
MODULE_API int zclk_argument_get_val_flag(zclk_argument *opt);

MODULE_API int zclk_argument_get_default_val_bool(zclk_argument *opt);
MODULE_API int zclk_argument_get_default_val_int(zclk_argument *opt);
MODULE_API double zclk_argument_get_default_val_double(zclk_argument *opt);
MODULE_API char* zclk_argument_get_default_val_string(zclk_argument *opt);
MODULE_API int zclk_argument_get_default_val_flag(zclk_argument *opt);

/**
 * Free resources used by argument
 */
MODULE_API void free_argument(zclk_argument* arg);

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
MODULE_API zclk_cmd_err make_command(zclk_command** command, char* name, 
	char* short_name, char* description, zclk_command_fn handler);

/**
 * @brief Create a command object with automatic error handling
 * @see make_command
 * 
 * @param name 
 * @param short_name 
 * @param description 
 * @param handler 
 * @return zclk_command* created command object
 */
MODULE_API zclk_command* new_zclk_command(
							char* name, 
							char* short_name,
    						char* description, 
							zclk_command_fn handler
						);

/**
 * @brief Add a subcommand to the given command
 * 
 * @param cmd command
 * @param subcommand subcommand to add
 * @return error code
 */
MODULE_API zclk_cmd_err zclk_command_subcommand_add(
							zclk_command *cmd,
							zclk_command *subcommand
						);

/**
 * @brief Add an option to the given command
 * 
 * @param cmd command
 * @param option option to add
 * @return error code
 */
MODULE_API zclk_cmd_err zclk_command_option_add(
							zclk_command *cmd,
							zclk_option* option
						);

/**
 * @brief Add an argument to the given command
 * 
 * @param cmd command
 * @param arg argument to add
 * @return error code
 */
MODULE_API zclk_cmd_err zclk_command_argument_add(
							zclk_command *cmd,
							zclk_argument* arg
						);

#define zclk_command_option_foreach(cmd, opt)     							\
		zclk_option *opt = NULL;                      						\
		size_t len##opt = arraylist_length(cmd->options);					\
		for (size_t i = 0; ((i < len##opt)?									\
							(opt = arraylist_get(cmd->options, i))			\
							: 0);											\
					i++)

#define zclk_command_argument_foreach(cmd, arg)    							\
		zclk_argument *arg = NULL;                     						\
		size_t len##arg = arraylist_length(cmd->args);						\
		for (size_t i = 0; ((i < len##arg)? 								\
							(arg = arraylist_get(cmd->args, i))				\
							: 0);											\
					i++)

/**
 * @brief Execute the command with the given args
 * 
 * @param cmd Command to execute
 * @param exec_args exec args
 * @param argc arg count
 * @param argv arg values
 * @return error code
 */
MODULE_API zclk_cmd_err zclk_command_exec(
	zclk_command *cmd,
	void *exec_args,
	int argc, char *argv[]);

/**
 * Free a command object
 */
MODULE_API void free_command(zclk_command* command);

/**
* Get help for a command
* \param cmds_to_exec the list of commands and subcommands parsed
* \return string with command help
*/
MODULE_API char* get_help_for_command(arraylist* cmds_to_exec);

/**
 * Run the help command for all commands or single command
 *
 * \param commands the list of commands registered 
 * 						(this is a list of zclk_command*)
 * \param handler_args an args value to be passed to the command handler
 * \param argc the number of tokens in the line
 * \param argv args as an array of strings
 * \param success_handler handle success results
 * \param error_handler handler error results
 */
MODULE_API zclk_cmd_err help_cmd_handler(arraylist* commands, 
	void* handler_args,	int argc, char** argv, 
	zclk_command_output_handler success_handler,
	zclk_command_output_handler error_handler);

/**
 * Get the help string for the arg_commands from the registered commands list.
 * \param help_str the help string to return
 * \param commands is the configured list of commands
 * \param arg_commands is a list of string
 * \return error code
 */
MODULE_API zclk_cmd_err get_help_for(char** help_str, arraylist* commands,
	arraylist* arg_commands);

/**
 * Execute a single line containing one top-level command.
 * All output is written to stdout, all errors to stderr
 *
 * \param commands the list of commands registered 
 * 						(this is a list of zclk_command*)
 * \param handler_args an args value to be passed to the command handler
 * \param argc the number of tokens in the line
 * \param argv args as an array of strings
 */
MODULE_API zclk_cmd_err exec_command(arraylist* commands, void* handler_args,
	int argc, char** argv);

/**
 * @brief Print a tabular result object to the stdout
 * 
 * @param result table result object
 */
MODULE_API void print_table_result(void* result);

/**
 * @brief A Print handler prints the result of the command
 * 
 * @param result_flag error flag
 * @param res_type result type
 * @param result result object
 * @return error code
 */
MODULE_API zclk_cmd_err print_handler(zclk_cmd_err result_flag, 
	zclk_result_type res_type,
    void* result);

#ifdef __cplusplus 
}
#endif

#endif /* __ZCLK_H__ */
