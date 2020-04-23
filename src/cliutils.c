/**
 * Copyright (c) 2020 Abhishek Mishra
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdlib.h>
#include <string.h>

#include "cliutils.h"

#define CLD_SIZE_OF_HELP_STR 4096
#define CLD_SIZE_OF_PROGNAME_STR 1024
static char help_str[CLD_SIZE_OF_HELP_STR];
static char progname_str[CLD_SIZE_OF_PROGNAME_STR];
static char short_progname_str[CLD_SIZE_OF_PROGNAME_STR];

void print_args(int argc, char **argv)
{
	for (int i = 0; i < argc; i++)
	{
		docker_log_debug("Arg%d = %s", i, argv[i]);
	}
}

/**
 * Create a new value object of given type.
 *
 * \param val object to create
 * \param type
 * \return error code
 */
cld_cmd_err make_cld_val(cld_val **val, cld_type type)
{
	(*val) = (cld_val *)calloc(1, sizeof(cld_val));
	if ((*val) == NULL)
	{
		return CLD_COMMAND_ERR_ALLOC_FAILED;
	}
	(*val)->type = type;
	clear_cld_val(*val);
	return CLD_COMMAND_SUCCESS;
}

cld_val* create_cld_val(cld_type type, int bool_val, int int_val, double dbl_val, char* str) {
	cld_val* v;
	make_cld_val(&v, type);
	if (v != NULL) {
		v->bool_value = bool_val;
		v->int_value = int_val;
		v->dbl_value = dbl_val;
		v->str_value = str;
	}
	return v;
}

/**
 * Free the created value
 */
void free_cld_val(cld_val *val)
{
	if (val->str_value)
	{
		free(val->str_value);
	}
	free(val);
}

/**
 * Reset values to system defaults.
 */
void clear_cld_val(cld_val *val)
{
	val->bool_value = 0;
	val->int_value = 0;
	val->dbl_value = 0.0;
	val->str_value = NULL;
}

/**
 * Copy values from 'from' to 'to'.
 * Can be used to reset to defaults.
 * (description and type are not copied)
 * str_value should be freed by caller.
 *
 * \param to val to set
 * \param from val to read from
 */
void copy_cld_val(cld_val *to, cld_val *from)
{
	to->bool_value = from->bool_value;
	to->int_value = from->int_value;
	to->dbl_value = from->dbl_value;
	to->str_value = from->str_value;
}

/**
 * Parse the input and read the value of the type of the val object.
 * (Should not be called when the values is a flag.)
 * The value should be set as soon as the argument/option is seen
 *
 * \param val object whose value will be set
 * \param input string input
 * \return error code
 */
cld_cmd_err parse_cld_val(cld_val *val, char *input)
{
	if (input == NULL)
	{
		return CLD_COMMAND_ERR_UNKNOWN;
	}
	else
	{
		int v;
		double d;
		switch (val->type)
		{
		case CLD_TYPE_BOOLEAN:
			sscanf(input, "%d", &v);
			if (v > 0)
			{
				val->bool_value = 1;
			}
			else
			{
				val->bool_value = 0;
			}
			break;
		case CLD_TYPE_INT:
			sscanf(input, "%d", &v);
			val->int_value = v;
			break;
		case CLD_TYPE_DOUBLE:
			sscanf(input, "%lf", &d);
			val->dbl_value = d;
			break;
		case CLD_TYPE_STRING:
			val->str_value = input;
			break;
		default:
			return CLD_COMMAND_ERR_UNKNOWN;
		}
		return CLD_COMMAND_SUCCESS;
	}
}

int cld_val_to_lua(lua_State *L, cld_val *val)
{
	if (val == NULL)
	{
		lua_pushnil(L);
	}
	else
	{
		switch (val->type)
		{
		case CLD_TYPE_FLAG:
		case CLD_TYPE_BOOLEAN:
			lua_pushboolean(L, val->bool_value);
			break;
		case CLD_TYPE_INT:
			lua_pushinteger(L, val->int_value);
			break;
		case CLD_TYPE_DOUBLE:
			lua_pushnumber(L, val->dbl_value);
			break;
		case CLD_TYPE_STRING:
			if (val->str_value == NULL) {
				lua_pushnil(L);
			}
			else {
				lua_pushstring(L, val->str_value);
			}
			break;
		default:
			lua_pushnil(L);
		}
	}
	return 1;
}

/**
 * Create a new option given a name and type.
 *
 * \param option object to create
 * \param name
 * \param short_name
 * \param type
 * \param description
 * \return error code
 */
cld_cmd_err make_option(cld_option **option, char *name, char *short_name,
	cld_val* val, cld_val* default_val, char *description)
{
	(*option) = (cld_option *)calloc(1, sizeof(cld_option));
	if ((*option) == NULL)
	{
		return CLD_COMMAND_ERR_ALLOC_FAILED;
	}
	(*option)->name = name;
	(*option)->short_name = short_name;
	(*option)->description = description;
	(*option)->val = val;
	(*option)->default_val = default_val;
	return CLD_COMMAND_SUCCESS;
}

cld_option* create_option(char* name, char* short_name, cld_val*val, cld_val* default_val, char* desc) {
	cld_option* o;
	make_option(&o, name, short_name, val, default_val, desc);
	return o;
}

/**
 * Free resources used by option
 */
void free_option(cld_option *option)
{
	if (option->short_name)
	{
		free(option->short_name);
	}
	if (option->description)
	{
		free(option->description);
	}

	free(option->val);
	if (option->default_val)
	{
		free(option->default_val);
	}
	free(option->name);
	free(option);
}

cld_option *get_option_by_name(arraylist *options, char *name)
{
	if (name != NULL)
	{
		size_t opt_len = arraylist_length(options);
		for (size_t i = 0; i < opt_len; i++)
		{
			cld_option *x = arraylist_get(options, i);
			if ((x->name != NULL && strcmp(x->name, name) == 0) || (x->short_name != NULL && strcmp(x->short_name, name) == 0))
			{
				return x;
			}
		}
	}
	return NULL;
}

int cld_option_to_lua(lua_State *L, cld_option *option)
{
	lua_createtable(L, 0, 5);

	if (option != NULL)
	{
		lua_pushstring(L, option->name);
		lua_setfield(L, -2, "name");

		if (option->short_name == NULL) {
			lua_pushnil(L);
		}
		else {
			lua_pushstring(L, option->short_name);
		}
		lua_setfield(L, -2, "short_name");

		cld_val_to_lua(L, option->val);
		lua_setfield(L, -2, "val");

		cld_val_to_lua(L, option->default_val);
		lua_setfield(L, -2, "default_val");

		if (option->description == NULL) {
			lua_pushnil(L);
		}
		else {
			lua_pushstring(L, option->description);
		}
		lua_setfield(L, -2, "description");
	}

	return 1;
}

void arraylist_cld_option_to_lua(lua_State *L, int index, void *data) {
	cld_option_to_lua(L, (cld_option*) data);
}

void cld_fill_options_in_list(arraylist* optlist, cld_option* options[]) {
	if (options != NULL && optlist != NULL) {
		int count = 0;
		cld_option* val;
		while ((val = options[count]) != NULL) {
			arraylist_add(optlist, val);
			count += 1;
		}
	}
}

/**
 * Create a new argument given a name and type.
 *
 * \param argument object to create
 * \param name
 * \param type
 * \param description
 * \return error code
 */
cld_cmd_err make_argument(cld_argument **arg, char* name, cld_val* val, cld_val* default_val, char* description)
{
	(*arg) = (cld_argument *)calloc(1, sizeof(cld_argument));
	if ((*arg) == NULL)
	{
		return CLD_COMMAND_ERR_ALLOC_FAILED;
	}
	(*arg)->name = name;
	(*arg)->description = description;
	(*arg)->optional = 0;
	(*arg)->val = val;
	(*arg)->default_val = default_val;
	return CLD_COMMAND_SUCCESS;
}

cld_argument* create_argument(char* name, cld_val* val, cld_val* default_val, char* desc) {
	cld_argument* arg;
	make_argument(&arg, name, val, default_val, desc);
	return arg;
}

/**
 * Free resources used by argument
 */
void free_argument(cld_argument *arg)
{
	if (arg->description)
	{
		free(arg->description);
	}
	free(arg->val);
	if (arg->default_val)
	{
		free(arg->default_val);
	}
	free(arg->name);
	free(arg);
}

int cld_argument_to_lua(lua_State *L, cld_argument* arg)
{
	lua_createtable(L, 0, 5);

	if (arg != NULL)
	{
		lua_pushstring(L, "name");
		lua_pushstring(L, arg->name);
		lua_settable(L, -3);

		lua_pushstring(L, "val");
		cld_val_to_lua(L, arg->val);
		lua_settable(L, -3);

		lua_pushstring(L, "default_val");
		cld_val_to_lua(L, arg->default_val);
		lua_settable(L, -3);

		lua_pushstring(L, "description");
		lua_pushstring(L, arg->description);
		lua_settable(L, -3);

		lua_pushstring(L, "optional");
		lua_pushboolean(L, arg->optional);
		lua_settable(L, -3);
	}

	return 1;
}

void arraylist_cld_argument_to_lua(lua_State *L, int index, void *data) {
	cld_argument_to_lua(L, (cld_argument*) data);
}

/**
 * Create a new command with the given name and handler
 * Options and sub-commands need to be added after creation.
 * The sub-commands and options lists will be initialized,
 * so one just needs to add items using the arraylist add function.
 *
 * \param command obj to be created
 * \param name
 * \param short_name
 * \param description
 * \param handler function ptr to handler
 * \return error code
 */
cld_cmd_err make_command(cld_command **command, char *name, char *short_name,
						 char *description, cld_command_handler handler)
{
	(*command) = (cld_command *)calloc(1, sizeof(cld_command));
	if ((*command) == NULL)
	{
		return CLD_COMMAND_ERR_ALLOC_FAILED;
	}
	(*command)->name = name;
	(*command)->short_name = short_name;
	(*command)->description = description;
	(*command)->handler = handler;
	arraylist_new(&((*command)->options), (void (*)(void *)) & free_option);
	set_lua_convertor((*command)->options, &arraylist_cld_option_to_lua);
	arraylist_new(&((*command)->sub_commands), (void (*)(void *)) & free_command);
	arraylist_new(&((*command)->args), (void (*)(void *)) & free_argument);
	set_lua_convertor((*command)->args, &arraylist_cld_argument_to_lua);
	return CLD_COMMAND_SUCCESS;
}

cld_command* create_command(char* name, char* short_name,
	char* description, cld_command_handler handler) {
	cld_command* cmd;
	if (make_command(&cmd, name, short_name, description, handler) == CLD_COMMAND_SUCCESS) {
		return cmd;
	}
	return NULL;
}

/**
 * Free a command object
 */
void free_command(cld_command *command)
{
	if (command->short_name)
	{
		free(command->short_name);
	}
	if (command->description)
	{
		free(command->description);
	}
	free(command->name);
	arraylist_free(command->options);
	arraylist_free(command->sub_commands);
	arraylist_free(command->args);
	free(command);
}

char *get_program_name(arraylist *cmds_to_exec)
{
	size_t cmd_len = arraylist_length(cmds_to_exec);
	memset(progname_str, 0, CLD_SIZE_OF_PROGNAME_STR);
	for (int i = 0; i < cmd_len; i++)
	{
		char *command = ((cld_command *)arraylist_get(cmds_to_exec, i))->name;
		char *p = command;
		for (int i = 0; i < strlen(command); i++)
		{
			if (command[i] == '/' || command[i] == '\\')
			{
				p = command + i + 1;
			}
		}
		strcat(progname_str, p);
		if (i != (cmd_len - 1))
		{
			strcat(progname_str, " ");
		}
	}
	return progname_str;
}

char *get_short_program_name(arraylist *cmds_to_exec)
{
	size_t cmd_len = arraylist_length(cmds_to_exec);
	memset(short_progname_str, 0, CLD_SIZE_OF_PROGNAME_STR);
	for (int i = 0; i < cmd_len; i++)
	{
		char *command = ((cld_command *)arraylist_get(cmds_to_exec, i))->short_name;
		char *p = command;
		for (int i = 0; i < strlen(command); i++)
		{
			if (command[i] == '/' || command[i] == '\\')
			{
				p = command + i + 1;
			}
		}
		strcat(short_progname_str, p);
		if (i != (cmd_len - 1))
		{
			strcat(short_progname_str, " ");
		}
	}
	return short_progname_str;
}

char *get_help_for_command(arraylist *cmds_to_exec)
{
	if (arraylist_length(cmds_to_exec) > 0)
	{
		cld_command *command = arraylist_get(cmds_to_exec, arraylist_length(cmds_to_exec) - 1);

		memset(help_str, 0, CLD_SIZE_OF_HELP_STR);
		sprintf(help_str, "Usage: %s", get_program_name(cmds_to_exec));

		size_t opt_len = arraylist_length(command->options);
		if (opt_len > 0)
		{
			strcat(help_str, " [OPTIONS]");
		}

		size_t sub_cmd_len = arraylist_length(command->sub_commands);
		if (sub_cmd_len > 0)
		{
			strcat(help_str, " COMMAND");
		}

		size_t cmd_args_len = arraylist_length(command->args);
		for (int ac = 0; ac < cmd_args_len; ac++)
		{
			cld_argument *arg = arraylist_get(command->args, ac);
			strcat(help_str, " <");
			strcat(help_str, arg->name);
			strcat(help_str, ">");
		}

		strcat(help_str, "\nOR:    ");
		strcat(help_str, get_short_program_name(cmds_to_exec));

		if (opt_len > 0)
		{
			strcat(help_str, " [OPTIONS]");
		}

		if (sub_cmd_len > 0)
		{
			strcat(help_str, " COMMAND");
		}

		for (int ac = 0; ac < cmd_args_len; ac++)
		{
			cld_argument *arg = arraylist_get(command->args, ac);
			strcat(help_str, " <");
			strcat(help_str, arg->name);
			strcat(help_str, ">");
		}

		strcat(help_str, "\n\n");
		strcat(help_str, command->description);
		strcat(help_str, "\n\n");

		if (opt_len > 0)
		{
			strcat(help_str, "Options:\n\n");
			for (size_t i = 0; i < opt_len; i++)
			{
				cld_option *opt = arraylist_get(command->options, i);
				strcat(help_str, "\t");
				if (opt->short_name != NULL)
				{
					strcat(help_str, "-");
					strcat(help_str, opt->short_name);
					strcat(help_str, ", ");
				}
				else
				{
					strcat(help_str, "    ");
				}

				size_t used = 0;
				if (opt->name != NULL)
				{
					strcat(help_str, "--");
					strcat(help_str, opt->name);
					used = 2 + strlen(opt->name);
					if (opt->val->type == CLD_TYPE_STRING)
					{
						strcat(help_str, " string");
						used += strlen(" string");
					}
				}
				for (size_t sp = used; sp < 25; sp++)
				{
					strcat(help_str, " ");
				}
				strcat(help_str, opt->description);
				strcat(help_str, "\n");
			}
			strcat(help_str, "\n");
		}

		if (sub_cmd_len > 0)
		{
			strcat(help_str, "\nCommands:\n\n");
			for (size_t i = 0; i < sub_cmd_len; i++)
			{
				cld_command *sc = arraylist_get(command->sub_commands, i);
				size_t used = 0;
				strcat(help_str, "  ");
				strcat(help_str, sc->name);
				used = 2 + strlen(sc->name);
				for (size_t sp = used; sp < 15; sp++)
				{
					strcat(help_str, " ");
				}
				strcat(help_str, sc->description);
				strcat(help_str, "\n");
			}
			strcat(help_str, "\n");
		}

		return help_str;
	}
	return NULL;
}

int gobble(int argc, char **argv, int at_pos)
{
	if (at_pos > (argc - 1))
	{
		return argc;
	}
	else
	{
		for (int i = at_pos; i < argc; i++)
		{
			if (i == argc)
			{
				argv[i] = NULL;
			}
			else
			{
				argv[i] = argv[i + 1];
			}
		}
		return argc - 1;
	}
}

arraylist *get_command_to_exec(arraylist *commands, int *argc,
							   char **argv)
{
	//First read all commands
	arraylist *cmd_names;
	arraylist_new(&cmd_names, &free);

	arraylist *cmds_to_exec = NULL;
	arraylist_new(&cmds_to_exec, NULL);

	cld_command *cmd_to_exec = NULL;

	arraylist *cmd_list = commands;
	while (1)
	{
		int found = 0;
		for (int i = 0; i < *argc; i++)
		{
			char *cmd_name = argv[i];
			if (cmd_list != NULL)
			{
				for (int j = 0; j < arraylist_length(cmd_list); j++)
				{
					cld_command *cmd = (cld_command *)arraylist_get(cmd_list,
																	j);
					if (strcmp(cmd_name, cmd->name) == 0 || strcmp(cmd_name, cmd->short_name) == 0)
					{
						found = 1;
						cmd_list = cmd->sub_commands;
						arraylist_add(cmd_names, str_clone(cmd->name));
						docker_log_debug("found command %s\n", cmd->name);
						cmd_to_exec = cmd;
						arraylist_add(cmds_to_exec, cmd);
						break;
					}
				}
			}
			if (found)
			{
				//gobble at location and break
				*argc = gobble(*argc, argv, i);
				break;
			}
		}
		//if no command found in last loop, break
		if (found == 0)
		{
			break;
		}
	}
	//arraylist_free(cmd_names);
	return cmds_to_exec;
}

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
cld_cmd_err help_cmd_handler(arraylist *commands, void *handler_args,
							 int argc, char **argv, cld_command_output_handler success_handler,
							 cld_command_output_handler error_handler)
{
	//First read all commands
	arraylist *cmds_to_exec = get_command_to_exec(commands, &argc, argv);
	if (arraylist_length(cmds_to_exec) > 0)
	{
		cld_command *cmd_to_exec = arraylist_get(cmds_to_exec, arraylist_length(cmds_to_exec) - 1);
		if (cmd_to_exec == NULL)
		{
			error_handler(CLD_COMMAND_ERR_COMMAND_NOT_FOUND, CLD_RESULT_STRING,
						  "No valid command found. Type help to get more help\n");
			return CLD_COMMAND_ERR_COMMAND_NOT_FOUND;
		}

		char *help_str = cmd_to_exec->description;
		success_handler(CLD_COMMAND_SUCCESS, CLD_RESULT_STRING, help_str);
	}
	return CLD_COMMAND_SUCCESS;
}

/**
 * Get the help string for the arg_commands from the registered commands list.
 * \param help_str the help string to return
 * \param commands is the configured list of commands
 * \param arg_commands is a list of string
 * \return error code
 */
cld_cmd_err get_help_for(char **help_str, arraylist *commands,
						 arraylist *arg_commands)
{
	arraylist *cmd_list = commands;
	for (int i = 0; i < arraylist_length(arg_commands); i++)
	{
		if (cmd_list != NULL)
		{
			char *cmd_name = arraylist_get(arg_commands, i);
			int found_cmd = 0;
			for (int j = 0; j < arraylist_length(cmd_list); j++)
			{
				cld_command *cmd = arraylist_get(cmd_list, j);
				if (strcmp(cmd_name, cmd->name) == 0)
				{
					found_cmd = 1;
					cmd_list = cmd->sub_commands;
					(*help_str) = cmd->description;
					break;
				}
			}
			if (found_cmd == 0)
			{
				return CLD_COMMAND_ERR_COMMAND_NOT_FOUND;
			}
		}
		else
		{
			return CLD_COMMAND_ERR_COMMAND_NOT_FOUND;
		}
	}
	return CLD_COMMAND_ERR_UNKNOWN;
}

cld_cmd_err parse_options(arraylist *options, int *argc, char **argv)
{
	while (1)
	{
		cld_option *found = NULL;
		for (int i = 0; i < *argc; i++)
		{
			char *option = argv[i];
			char *long_option_name = NULL;
			char *short_option_name = NULL;
			if (strlen(option) > 1 && option[0] == '-')
			{
				int skip_count = 0;
				if (option[1] == '-')
				{
					//long option
					long_option_name = option + 2;
				}
				else
				{
					//short option
					short_option_name = option + 1;
				}
				size_t options_len = arraylist_length(options);
				for (int j = 0; j < options_len; j++)
				{
					cld_option *opt = arraylist_get(options, j);
					if (long_option_name && opt->name != NULL)
					{
						if (strcmp(long_option_name, opt->name) == 0)
						{
							found = opt;
						}
					}
					if (short_option_name && opt->short_name != NULL)
					{
						if (strcmp(short_option_name, opt->short_name) == 0)
						{
							found = opt;
						}
					}
				}
				if (found == NULL)
				{
					printf("Unknown option %s\n.", option);
					return CLD_COMMAND_ERR_OPTION_NOT_FOUND;
				}
				else
				{
					//printf("Found %s\n", found->name);
					skip_count += 1;
					//read option value if it is not a flag
					if (found->val->type == CLD_TYPE_FLAG)
					{
						found->val->bool_value = true;
					}
					else
					{
						if (i == (*argc - 1))
						{
							printf("Value missing for option %s.\n", option);
							return CLD_COMMAND_ERR_OPTION_NOT_FOUND;
						}
						else
						{
							skip_count += 1;
							char *value = argv[i + 1];
							parse_cld_val(found->val, value);
						}
					}
					for (int sk = 0; sk < skip_count; sk++)
					{
						//gobble at location and break
						//printf("GOBBLING FOR %s, argc = %d\n", found->name, *argc);
						*argc = gobble(*argc, argv, i);
					}
					break;
				}
			}
		}
		if (found == NULL)
		{
			// no option found in last loop
			break;
		}
	}

	return CLD_COMMAND_SUCCESS;
}

cld_cmd_err parse_args(arraylist *args, int *argc, char **argv)
{
	size_t args_len = arraylist_length(args);
	//	printf("args len %d\n", args_len);
	if (args_len == *argc)
	{
		for (int i = 0; i < args_len; i++)
		{
			cld_argument *arg = arraylist_get(args, i);
			char *argval = argv[i];
			//check if we have
			parse_cld_val(arg->val, argval);
			//			printf("Argval %s\n", argval);
		}
	}
	else
	{
		return CLD_COMMAND_ERR_ARG_NOT_FOUND;
	}
	for (int i = 0; i < args_len; i++)
	{
		(*argc) = gobble((*argc), &argv, 0);
		//		printf("new argc = %d\n", (*argc));
	}
	return CLD_COMMAND_SUCCESS;
}

void print_options(arraylist *options)
{
	if (options != NULL)
	{
		size_t options_len = arraylist_length(options);
		for (int i = 0; i < options_len; i++)
		{
			cld_option *o = arraylist_get(options, i);
			switch (o->val->type)
			{
			case CLD_TYPE_BOOLEAN:
				docker_log_debug("Options%d %s, %s = %d\n", i, o->name, o->short_name, o->val->bool_value);
				break;
			case CLD_TYPE_FLAG:
				docker_log_debug("Options%d %s, %s = %d\n", i, o->name, o->short_name, o->val->bool_value);
				break;
			case CLD_TYPE_STRING:
				docker_log_debug("Options%d %s, %s = %s\n", i, o->name, o->short_name, o->val->str_value);
				break;
			}
		}
	}
}

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
cld_cmd_err exec_command(arraylist *commands, void *handler_args,
						 int argc, char **argv, cld_command_output_handler success_handler,
						 cld_command_output_handler error_handler)
{
	cld_cmd_err err = CLD_COMMAND_SUCCESS;

	//First read all commands
	arraylist *cmds_to_exec = get_command_to_exec(commands, &argc, argv);
	size_t len_cmds = arraylist_length(cmds_to_exec);
	arraylist *all_options, *all_args;
	arraylist_new(&all_options, NULL);
	set_lua_convertor(all_options, &arraylist_cld_option_to_lua);
	arraylist_new(&all_args, NULL);
	set_lua_convertor(all_args, &arraylist_cld_argument_to_lua);

	for (int i = 0; i < len_cmds; i++)
	{
		cld_command *cmd_to_exec = arraylist_get(cmds_to_exec, i);
		size_t len_options = arraylist_length(cmd_to_exec->options);
		for (int j = 0; j < len_options; j++)
		{
			arraylist_add(all_options, arraylist_get(cmd_to_exec->options, j));
		}
		size_t len_args = arraylist_length(cmd_to_exec->args);
		for (int j = 0; j < len_args; j++)
		{
			arraylist_add(all_args, arraylist_get(cmd_to_exec->args, j));
		}
	}

	print_args(argc, argv);

	//Then read all options
	err = parse_options(all_options, &argc, argv);
	if (err != CLD_COMMAND_SUCCESS)
	{
		return err;
	}

	print_options(all_options);

	cld_option *help_option = get_option_by_name(all_options, CLD_OPTION_HELP_LONG);
	if (help_option->val->bool_value)
	{
		char *help_str = get_help_for_command(cmds_to_exec);
		if (help_str == NULL)
		{
			error_handler(CLD_COMMAND_ERR_COMMAND_NOT_FOUND, CLD_RESULT_STRING,
						  "No valid command found. Type help to get more help\n");
			return CLD_COMMAND_ERR_COMMAND_NOT_FOUND;
		}
		success_handler(CLD_COMMAND_SUCCESS, CLD_RESULT_STRING, help_str);
	}
	else
	{
		for (int i = 0; i < len_cmds; i++)
		{
			cld_command *cmd_to_exec = arraylist_get(cmds_to_exec, i);
			if (cmd_to_exec == NULL)
			{
				printf("No valid command found. Type help to get more help\n");
				return CLD_COMMAND_ERR_COMMAND_NOT_FOUND;
			}

			// for the last command in the chain, it can have args
			if (i == (len_cmds - 1))
			{
				//Now read all arguments
				err = parse_args(cmd_to_exec->args, &argc, argv);
				if (err != CLD_COMMAND_SUCCESS)
				{
					return err;
				}

				//anything leftover
				if (argc > 0)
				{
					printf("%d extra arguments found.\n", argc);
					return CLD_COMMAND_ERR_EXTRA_ARGS_FOUND;
				}
			}

			if (cmd_to_exec->handler != NULL)
			{
				err = cmd_to_exec->handler(handler_args, all_options,
										   all_args, success_handler, error_handler);
			}
		}
	}

	arraylist_free(cmds_to_exec);
	arraylist_free(all_options);
	arraylist_free(all_args);
	return err;
}
