/**
 * Copyright (c) 2020 Abhishek Mishra
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdlib.h>
#include <string.h>

#include "zclk.h"

#define ZCLK_SIZE_OF_HELP_STR 4096
#define ZCLK_SIZE_OF_PROGNAME_STR 1024
static char help_str[ZCLK_SIZE_OF_HELP_STR];
static char progname_str[ZCLK_SIZE_OF_PROGNAME_STR];
static char short_progname_str[ZCLK_SIZE_OF_PROGNAME_STR];

static char error_message_str[ZCLK_SIZE_OF_HELP_STR];

void print_args(int argc, char **argv)
{
	for (int i = 0; i < argc; i++)
	{
		printf("Arg%d = %s", i, argv[i]);
	}
}

zclk_res make_zclk_val(zclk_val **val, zclk_type type)
{
	(*val) = (zclk_val *)calloc(1, sizeof(zclk_val));
	if ((*val) == NULL)
	{
		return ZCLK_RES_ERR_ALLOC_FAILED;
	}
	(*val)->type = type;
	return ZCLK_RES_SUCCESS;
}

zclk_val* new_zclk_val_bool(int bool_val)
{
	zclk_val *val;
	zclk_res res = make_zclk_val(&val, ZCLK_TYPE_BOOLEAN);
	if(res == ZCLK_RES_SUCCESS)
	{
		val->data.bool_value = bool_val;
	}
	return val;
}

zclk_val* new_zclk_val_int(int int_val)
{
	zclk_val *val;
	zclk_res res = make_zclk_val(&val, ZCLK_TYPE_INT);
	if(res == ZCLK_RES_SUCCESS)
	{
		val->data.int_value = int_val;
	}
	return val;
}

zclk_val* new_zclk_val_double(double double_val)
{
	zclk_val *val;
	zclk_res res = make_zclk_val(&val, ZCLK_TYPE_DOUBLE);
	if(res == ZCLK_RES_SUCCESS)
	{
		val->data.dbl_value = double_val;
	}
	return val;
}

zclk_val* new_zclk_val_string(const char* string_val)
{
	zclk_val *val;
	zclk_res res = make_zclk_val(&val, ZCLK_TYPE_STRING);
	if(res == ZCLK_RES_SUCCESS)
	{
		val->data.str_value = zclk_str_clone(string_val);
	}
	return val;
}

zclk_val* new_zclk_val_flag(int flag_val)
{
	zclk_val *val;
	zclk_res res = make_zclk_val(&val, ZCLK_TYPE_FLAG);
	if(res == ZCLK_RES_SUCCESS)
	{
		val->data.bool_value = flag_val;
	}
	return val;
}

int zclk_val_is_type(zclk_val *val, zclk_type type)
{
	if (val == NULL)
	{
		return 0;
	}
	if(val->type == type)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int zclk_val_get_bool(zclk_val *val)
{
	return (int)(val->data.bool_value);
}

int zclk_val_get_int(zclk_val *val)
{

	return (int)(val->data.int_value);
}

double zclk_val_get_double(zclk_val *val)
{
	return (double)(val->data.dbl_value);
}

const char *zclk_val_get_string(zclk_val * val)
{
	return (const char *)(val->data.str_value);
}

int zclk_val_get_flag(zclk_val * val)
{
	return (int)(val->data.bool_value);
}

void zclk_val_set_bool(zclk_val *val, int bval)
{
	if(val!= NULL)
	{
		val->data.bool_value = bval;
	}
}

void zclk_val_set_int(zclk_val *val, int ival)
{
	if(val!= NULL)
	{
		val->data.int_value = ival;
	}
}
void zclk_val_set_dobule(zclk_val *val, double dval)
{
	if(val!= NULL)
	{
		val->data.dbl_value = dval;
	}
}

void zclk_val_set_string(zclk_val *val, const char* sval)
{
	if(val!= NULL)
	{
		val->data.str_value = zclk_str_clone(sval);
	}
}

void zclk_val_set_flag(zclk_val *val, int fval)
{
	if(val!= NULL)
	{
		val->data.bool_value = fval;
	}
}

void free_zclk_val(zclk_val * val)
{
	if (zclk_val_is_string(val))
	{
		free(val->data.str_value);
	}
	free(val);
}

void copy_zclk_val(zclk_val *to, zclk_val *from)
{
	to->type = from->type;
	if(zclk_val_is_bool(from))
	{
		to->data.bool_value = from->data.bool_value;
	}
	if(zclk_val_is_int(from))
	{
		to->data.int_value = from->data.int_value;
	}
	if(zclk_val_is_double(from))
	{
		to->data.dbl_value = from->data.dbl_value;
	}
	if(zclk_val_is_string(from))
	{
		to->data.str_value = from->data.str_value;
	}
	if(zclk_val_is_flag(from))
	{
		to->data.bool_value = from->data.bool_value;
	}
}

zclk_res parse_zclk_val(zclk_val *val, char *input)
{
	if (input == NULL)
	{
		return ZCLK_RES_ERR_UNKNOWN;
	}
	else
	{
		int v, n;
		double d;
		switch (val->type)
		{
		case ZCLK_TYPE_BOOLEAN:
			n = sscanf(input, "%d", &v);
			if (n == 1)
			{
				if (v > 0)
				{
					zclk_val_set_bool(val, 1);
				}
				else
				{
					zclk_val_set_bool(val, 0);
				}
			}
			break;
		case ZCLK_TYPE_INT:
			n = sscanf(input, "%d", &v);
			if (n == 1)
			{
				zclk_val_set_int(val, v);
			}
			break;
		case ZCLK_TYPE_DOUBLE:
			n = sscanf(input, "%lf", &d);
			if (n == 1)
			{
				zclk_val_set_dobule(val, d);
			}
			break;
		case ZCLK_TYPE_STRING:
			zclk_val_set_string(val, input);
			break;
		default:
			return ZCLK_RES_ERR_UNKNOWN;
		}
		return ZCLK_RES_SUCCESS;
	}
}

#ifdef LUA_ENABLED
int zclk_val_to_lua(lua_State *L, zclk_val *val)
{
	if (val == NULL)
	{
		lua_pushnil(L);
	}
	else
	{
		switch (val->type)
		{
		case ZCLK_TYPE_FLAG:
		case ZCLK_TYPE_BOOLEAN:
			lua_pushboolean(L, zclk_val_get_bool(val));
			break;
		case ZCLK_TYPE_INT:
			lua_pushinteger(L, zclk_val_get_int(val));
			break;
		case ZCLK_TYPE_DOUBLE:
			lua_pushnumber(L, zclk_val_get_double(val));
			break;
		case ZCLK_TYPE_STRING:
			if (zclk_val_get_string(val) == NULL) {
				lua_pushnil(L);
			}
			else {
				lua_pushstring(L, zclk_val_get_string(val));
			}
			break;
		default:
			lua_pushnil(L);
		}
	}
	return 1;
}
#endif //LUA_ENABLED

zclk_res make_option(zclk_option **option, const char *name, const char *short_name,
	zclk_val* val, zclk_val* default_val, const char *description)
{
	(*option) = (zclk_option *)calloc(1, sizeof(zclk_option));
	if ((*option) == NULL)
	{
		return ZCLK_RES_ERR_ALLOC_FAILED;
	}
	(*option)->name = zclk_str_clone(name);
	(*option)->short_name = zclk_str_clone(short_name);
	(*option)->description = zclk_str_clone(description);
	(*option)->val = val;
	(*option)->default_val = default_val;
	return ZCLK_RES_SUCCESS;
}

zclk_option* new_zclk_option(const char* name, const char* short_name, 
	zclk_val*val, zclk_val* default_val, const char* desc) 
{
	zclk_option* o;
	make_option(&o, name, short_name, val, default_val, desc);
	return o;
}

zclk_option *new_zclk_option_bool(const char *name, const char *short_name, 
	int default_val, const char *desc)
{
	return new_zclk_option(name, short_name, 
		new_zclk_val_bool(default_val), new_zclk_val_bool(default_val), desc);
}

zclk_option *new_zclk_option_int(const char *name, const char *short_name, 
	int default_val, const char *desc)
{
	return new_zclk_option(name, short_name, 
		new_zclk_val_int(default_val), new_zclk_val_int(default_val), desc);
}

zclk_option *new_zclk_option_double(const char *name, const char *short_name, 
	double default_val, const char *desc)
{
	return new_zclk_option(name, short_name, 
		new_zclk_val_double(default_val), new_zclk_val_double(default_val), 
		desc);
}

zclk_option *new_zclk_option_string(const char *name, const char *short_name, 
	const char *default_val, const char *desc)
{
	return new_zclk_option(name, short_name, 
		new_zclk_val_string(default_val), new_zclk_val_string(default_val), 
		desc);
}

zclk_option *new_zclk_option_flag(const char *name, const char *short_name, 
	int default_val, const char *desc)
{
	return new_zclk_option(name, short_name, 
		new_zclk_val_flag(default_val), new_zclk_val_flag(default_val), desc);
}

const char *zclk_option_get_name(zclk_option *opt)
{
	if(opt == NULL)
	{
		return NULL;
	}
	return (opt->name);
}

const char *zclk_option_get_short_name(zclk_option *opt)
{
	if(opt == NULL)
	{
		return NULL;
	}
	return (opt->name);
}

const char *zclk_option_get_desc(zclk_option *opt)
{
	if(opt == NULL)
	{
		return NULL;
	}
	return (opt->name);
}

int zclk_option_get_val_bool(zclk_option *opt)
{
	if(opt == NULL)
	{
		return 0;
	}
	return zclk_val_get_bool(opt->val);
}

int zclk_option_get_val_int(zclk_option *opt)
{
	if(opt == NULL)
	{
		return 0;
	}
	return zclk_val_get_int(opt->val);
}

double zclk_option_get_val_double(zclk_option *opt)
{
	if(opt == NULL)
	{
		return 0;
	}
	return zclk_val_get_double(opt->val);
}

const char* zclk_option_get_val_string(zclk_option *opt)
{
	if(opt == NULL)
	{
		return NULL;
	}
	return zclk_val_get_string(opt->val);
}

int zclk_option_get_val_flag(zclk_option *opt)
{
	if(opt == NULL)
	{
		return 0;
	}
	return zclk_val_get_flag(opt->val);
}

int zclk_option_get_default_val_bool(zclk_option *opt)
{
	if(opt == NULL)
	{
		return 0;
	}
	return zclk_val_get_bool(opt->default_val);
}

int zclk_option_get_default_val_int(zclk_option *opt)
{
	if(opt == NULL)
	{
		return 0;
	}
	return zclk_val_get_int(opt->default_val);
}

double zclk_option_get_default_val_double(zclk_option *opt)
{
	if(opt == NULL)
	{
		return 0;
	}
	return zclk_val_get_double(opt->default_val);
}

const char* zclk_option_get_default_val_string(zclk_option *opt)
{
	if(opt == NULL)
	{
		return 0;
	}
	return zclk_val_get_string(opt->default_val);
}

int zclk_option_get_default_val_flag(zclk_option *opt)
{
	if(opt == NULL)
	{
		return 0;
	}
	return zclk_val_get_flag(opt->default_val);
}

void free_option(zclk_option *option)
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

zclk_option *get_option_by_name(arraylist *options, const char *name)
{
	if (name != NULL)
	{
		size_t opt_len = arraylist_length(options);
		for (size_t i = 0; i < opt_len; i++)
		{
			zclk_option *x = arraylist_get(options, i);
			if ((x->name != NULL && strcmp(x->name, name) == 0) ||
			 (x->short_name != NULL && strcmp(x->short_name, name) == 0))
			{
				return x;
			}
		}
	}
	return NULL;
}

#ifdef LUA_ENABLED
int zclk_option_to_lua(lua_State *L, zclk_option *option)
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

		zclk_val_to_lua(L, option->val);
		lua_setfield(L, -2, "val");

		zclk_val_to_lua(L, option->default_val);
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

void arraylist_zclk_option_to_lua(lua_State *L, int index, void *data) {
	zclk_option_to_lua(L, (zclk_option*) data);
}
#endif //LUA_ENABLED

void zclk_fill_options_in_list(arraylist* optlist, zclk_option* options[]) {
	if (options != NULL && optlist != NULL) {
		int count = 0;
		zclk_option* val;
		while ((val = options[count]) != NULL) {
			arraylist_add(optlist, val);
			count += 1;
		}
	}
}

zclk_res make_argument(zclk_argument **arg, const char* name, zclk_val* val, 
	zclk_val* default_val, const char* description)
{
	(*arg) = (zclk_argument *)calloc(1, sizeof(zclk_argument));
	if ((*arg) == NULL)
	{
		return ZCLK_RES_ERR_ALLOC_FAILED;
	}
	(*arg)->name = zclk_str_clone(name);
	(*arg)->description = zclk_str_clone(description);
	(*arg)->optional = 0;
	(*arg)->val = val;
	(*arg)->default_val = default_val;
	return ZCLK_RES_SUCCESS;
}

zclk_argument* new_zclk_argument(const char* name, zclk_val* val, 
			zclk_val* default_val, const char* desc, int nargs) {
	zclk_argument* arg;
	make_argument(&arg, name, val, default_val, desc);
	return arg;
}

zclk_argument *new_zclk_argument_bool(const char *name, 
	int default_val, const char *desc, int nargs)
{
	return new_zclk_argument(name, 
		new_zclk_val_bool(default_val), 
			new_zclk_val_bool(default_val), desc, nargs);
}

zclk_argument *new_zclk_argument_int(const char *name, 
	int default_val, const char *desc, int nargs)
{
	return new_zclk_argument(name, 
		new_zclk_val_int(default_val), 
			new_zclk_val_int(default_val), desc, nargs);
}

zclk_argument *new_zclk_argument_double(const char *name, 
	double default_val, const char *desc, int nargs)
{
	return new_zclk_argument(name, 
		new_zclk_val_double(default_val), 
		new_zclk_val_double(default_val), desc, nargs);
}

zclk_argument *new_zclk_argument_string(const char *name, 
	const char *default_val, const char *desc, int nargs)
{
	return new_zclk_argument(name, 
		new_zclk_val_string(default_val), 
		new_zclk_val_string(default_val), desc, nargs);
}

zclk_argument *new_zclk_argument_flag(const char *name, 
	int default_val, const char *desc, int nargs)
{
	return new_zclk_argument(name, 
		new_zclk_val_flag(default_val), 
			new_zclk_val_flag(default_val), desc, nargs);
}


const char *zclk_argument_get_name(zclk_argument *arg)
{
	if(arg == NULL)
	{
		return NULL;
	}
	return (arg->name);
}

const char *zclk_argument_get_desc(zclk_argument *arg)
{
	if(arg == NULL)
	{
		return NULL;
	}
	return (arg->description);
}


int zclk_argument_get_val_bool(zclk_argument *arg)
{
	if(arg == NULL)
	{
		return 0;
	}
	return zclk_val_get_bool(arg->val);
}

int zclk_argument_get_val_int(zclk_argument *arg)
{
	if(arg == NULL)
	{
		return 0;
	}
	return zclk_val_get_int(arg->val);
}

double zclk_argument_get_val_double(zclk_argument *arg)
{
	if(arg == NULL)
	{
		return 0;
	}
	return zclk_val_get_double(arg->val);
}

const char* zclk_argument_get_val_string(zclk_argument *arg)
{
	if(arg == NULL)
	{
		return 0;
	}
	return zclk_val_get_string(arg->val);
}

int zclk_argument_get_val_flag(zclk_argument *arg)
{
	if(arg == NULL)
	{
		return 0;
	}
	return zclk_val_get_flag(arg->val);
}

int zclk_argument_get_default_val_bool(zclk_argument *arg)
{
	if(arg == NULL)
	{
		return 0;
	}
	return zclk_val_get_bool(arg->default_val);
}

int zclk_argument_get_default_val_int(zclk_argument *arg)
{
	if(arg == NULL)
	{
		return 0;
	}
	return zclk_val_get_int(arg->default_val);
}

double zclk_argument_get_default_val_double(zclk_argument *arg)
{
	if(arg == NULL)
	{
		return 0;
	}
	return zclk_val_get_double(arg->default_val);
}

const char* zclk_argument_get_default_val_string(zclk_argument *arg)
{
	if(arg == NULL)
	{
		return 0;
	}
	return zclk_val_get_string(arg->default_val);
}

int zclk_argument_get_default_val_flag(zclk_argument *arg)
{
	if(arg == NULL)
	{
		return 0;
	}
	return zclk_val_get_flag(arg->default_val);
}


void free_argument(zclk_argument *arg)
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

#ifdef LUA_ENABLED
int zclk_argument_to_lua(lua_State *L, zclk_argument* arg)
{
	lua_createtable(L, 0, 5);

	if (arg != NULL)
	{
		lua_pushstring(L, "name");
		lua_pushstring(L, arg->name);
		lua_settable(L, -3);

		lua_pushstring(L, "val");
		zclk_val_to_lua(L, arg->val);
		lua_settable(L, -3);

		lua_pushstring(L, "default_val");
		zclk_val_to_lua(L, arg->default_val);
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

void arraylist_zclk_argument_to_lua(lua_State *L, int index, void *data) {
	zclk_argument_to_lua(L, (zclk_argument*) data);
}
#endif //LUA_ENABLED

zclk_res make_command(zclk_command **command, const char *name, const char *short_name,
						 const char *description, zclk_command_fn handler)
{
	(*command) = (zclk_command *)calloc(1, sizeof(zclk_command));
	if ((*command) == NULL)
	{
		return ZCLK_RES_ERR_ALLOC_FAILED;
	}
	(*command)->name = zclk_str_clone(name);
	(*command)->short_name = zclk_str_clone(short_name);
	(*command)->description = zclk_str_clone(description);
	(*command)->handler = handler;
	(*command)->error_handler = (zclk_command_output_handler)&print_handler;
	(*command)->success_handler = (zclk_command_output_handler)&print_handler;

	arraylist_new(&((*command)->options), (void (*)(void *)) & free_option);
	#ifdef LUA_ENABLED
		set_lua_convertor((*command)->options, &arraylist_zclk_option_to_lua);
	#endif //LUA_ENABLED
	//arraylist_new(&((*command)->sub_commands), (void (*)(void *)) & free_command);
	// sub-commands are to be freed individually as this works with the lua
	// automatic garbage collection model.
	// otherwise if sub-commands are freed by command gc call,
	// then sub-command free fails with dangling pointer...
	arraylist_new(&((*command)->sub_commands), NULL);
	arraylist_new(&((*command)->args), (void (*)(void *)) & free_argument);

	#ifdef LUA_ENABLED
		set_lua_convertor((*command)->args, &arraylist_zclk_argument_to_lua);
	#endif //LUA_ENABLED

	zclk_command_option_add(
		(*command),
		new_zclk_option(ZCLK_OPTION_HELP_LONG,
					  ZCLK_OPTION_HELP_SHORT, zclk_flag(0),
					  zclk_flag(0), ZCLK_OPTION_HELP_DESC));

	return ZCLK_RES_SUCCESS;
}

zclk_command* new_zclk_command(const char* name, const char* short_name,
	const char* description, zclk_command_fn handler) {
	zclk_command* cmd;
	if (make_command(&cmd, name, short_name, description, handler) == ZCLK_RES_SUCCESS) {
		return cmd;
	}
	return NULL;
}

zclk_res zclk_command_subcommand_add(zclk_command *cmd, 
											zclk_command *subcommand)
{
	if(cmd == NULL || subcommand == NULL) 
	{
		return ZCLK_RES_ERR_UNKNOWN;
	}

	arraylist_add(cmd->sub_commands, subcommand);
	return ZCLK_RES_SUCCESS;
}

zclk_res zclk_command_option_add(
							zclk_command *cmd,
							zclk_option* option
						)
{
	if(cmd == NULL || option == NULL) 
	{
		return ZCLK_RES_ERR_UNKNOWN;
	}

	arraylist_add(cmd->options, option);
	return ZCLK_RES_SUCCESS;
}

zclk_res zclk_command_argument_add(
							zclk_command *cmd,
							zclk_argument* arg
						)
{
	if(cmd == NULL || arg == NULL) 
	{
		return ZCLK_RES_ERR_UNKNOWN;
	}

	arraylist_add(cmd->args, arg);
	return ZCLK_RES_SUCCESS;
}

void zclk_command_bool_option(zclk_command *cmd, const char *name, 
				const char* short_name, const char *desc)
{
	zclk_command_option_add(cmd, 
			new_zclk_option_bool(
					name,
					short_name,
					0,
					desc
				));
}

void zclk_command_int_option(zclk_command *cmd, const char *name, 
				const char* short_name, int default_val, const char *desc)
{
	zclk_command_option_add(cmd, 
			new_zclk_option_int(
					name,
					short_name,
					default_val,
					desc
				));
}


void zclk_command_double_option(zclk_command *cmd, const char *name, 
				const char* short_name, double default_val, const char *desc)
{
	zclk_command_option_add(cmd, 
			new_zclk_option_double(
					name,
					short_name,
					default_val,
					desc
				));
}


void zclk_command_string_option(zclk_command *cmd, const char *name, 
				const char* short_name, const char *default_val, 
				const char *desc)
{
	zclk_command_option_add(cmd, 
			new_zclk_option_string(
					name,
					short_name,
					default_val,
					desc
				));
}

void zclk_command_flag_option(zclk_command *cmd, const char *name, 
				const char* short_name, const char *desc)
{
	zclk_command_option_add(cmd, 
			new_zclk_option_flag(
					name,
					short_name,
					0,
					desc
				));
}


void zclk_command_bool_argument(zclk_command *cmd, const char *name, 
				int default_val, const char *desc, int nargs)
{
	zclk_command_argument_add(cmd,
			new_zclk_argument_bool(
				name,
				default_val,
				desc,
				nargs));
}

void zclk_command_int_argument(zclk_command *cmd, const char *name, 
				int default_val, const char *desc, int nargs)
{
	zclk_command_argument_add(cmd,
			new_zclk_argument_int(
				name,
				default_val,
				desc,
				nargs));
}


void zclk_command_double_argument(zclk_command *cmd, const char *name, 
				double default_val, const char *desc, int nargs)
{
	zclk_command_argument_add(cmd,
			new_zclk_argument_double(
				name,
				default_val,
				desc,
				nargs));
}


void zclk_command_string_argument(zclk_command *cmd, const char *name, 
				const char *default_val, const char *desc, int nargs)
{
	zclk_command_argument_add(cmd,
			new_zclk_argument_string(
				name,
				default_val,
				desc,
				nargs));
}


void zclk_command_flag_argument(zclk_command *cmd, const char *name, 
				int default_val, const char *desc, int nargs)
{
	zclk_command_argument_add(cmd,
			new_zclk_argument_flag(
				name,
				default_val,
				desc,
				nargs));
}

zclk_option* zclk_command_get_option(zclk_command *cmd, const char *name)
{
	if(cmd != NULL && name != NULL)
	{
		size_t opt_len = arraylist_length(cmd->options);
		for (size_t i = 0; i < opt_len; i++)
		{
			zclk_option *x = arraylist_get(cmd->options, i);
			if ((x->name != NULL && strcmp(x->name, name) == 0) 
				|| (x->short_name != NULL 
						&& strcmp(x->short_name, name) == 0))
			{
				return x;
			}
		}
	}
	return NULL;
}

zclk_argument* zclk_command_get_argument(zclk_command *cmd, const char *name)
{
	if(cmd != NULL && name != NULL)
	{
		size_t opt_len = arraylist_length(cmd->args);
		for (size_t i = 0; i < opt_len; i++)
		{
			zclk_argument *x = arraylist_get(cmd->args, i);
			if (x->name != NULL && strcmp(x->name, name) == 0)
			{
				return x;
			}
		}
	}
	return NULL;
}


zclk_res zclk_command_exec(zclk_command* cmd, 
	void* exec_args, int argc, char* argv[])
{
	arraylist *toplevel_commands;
	arraylist_new(&toplevel_commands, NULL);
	arraylist_add(toplevel_commands, cmd);
	zclk_res err = exec_command(toplevel_commands, 
										exec_args, argc, argv);
	if (err != ZCLK_RES_SUCCESS)
	{
		//printf("Error: invalid command. Error code: %d\n", err);
		printf("Error: ");
		printf("%s", error_message_str);
		printf("\n\n");
		if(err != ZCLK_RES_ERR_COMMAND_NOT_FOUND)
		{
			char* help_message_str = get_help_for_command(toplevel_commands);
			printf("%s", help_message_str);
		}
	}
	return err;
}

void free_command(zclk_command *command)
{
	if(command != NULL)
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
}

char *get_program_name(arraylist *cmds_to_exec)
{
	size_t cmd_len = arraylist_length(cmds_to_exec);
	memset(progname_str, 0, ZCLK_SIZE_OF_PROGNAME_STR);
	for (int i = 0; i < cmd_len; i++)
	{
		char *command = ((zclk_command *)arraylist_get(cmds_to_exec, i))->name;
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
	memset(short_progname_str, 0, ZCLK_SIZE_OF_PROGNAME_STR);
	for (int i = 0; i < cmd_len; i++)
	{
		char *command = ((zclk_command *)arraylist_get(cmds_to_exec, i))->short_name;
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
		zclk_command *command = arraylist_get(cmds_to_exec, 
			arraylist_length(cmds_to_exec) - 1);

		memset(help_str, 0, ZCLK_SIZE_OF_HELP_STR);
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
			zclk_argument *arg = arraylist_get(command->args, ac);
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
			zclk_argument *arg = arraylist_get(command->args, ac);
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
				zclk_option *opt = arraylist_get(command->options, i);
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
					if (opt->val->type == ZCLK_TYPE_STRING)
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
				zclk_command *sc = arraylist_get(command->sub_commands, i);
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
			// printf("--\t %d, %d, %s\n", i, at_pos, argv[i]);
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

	zclk_command *cmd_to_exec = NULL;

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
					zclk_command *cmd = (zclk_command *)arraylist_get(cmd_list,
																	j);
					if (strcmp(cmd_name, cmd->name) == 0 
						|| strcmp(cmd_name, cmd->short_name) == 0)
					{
						found = 1;
						cmd_list = cmd->sub_commands;
						arraylist_add(cmd_names, zclk_str_clone(cmd->name));
						//printf("found command %s\n", cmd->name);
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

zclk_res help_cmd_handler(arraylist *commands, void *handler_args,
		int argc, char **argv, zclk_command_output_handler success_handler,
		zclk_command_output_handler error_handler)
{
	//First read all commands
	arraylist *cmds_to_exec = get_command_to_exec(commands, &argc, argv);
	if (arraylist_length(cmds_to_exec) > 0)
	{
		zclk_command *cmd_to_exec = 
			arraylist_get(cmds_to_exec, arraylist_length(cmds_to_exec) - 1);
		if (cmd_to_exec == NULL)
		{
			error_handler(ZCLK_RES_ERR_COMMAND_NOT_FOUND, ZCLK_RESULT_STRING,
				"No valid command found. Run again with" \
				" --help to see usage.\n");
			
			return ZCLK_RES_ERR_COMMAND_NOT_FOUND;
		}

		char *help_str = cmd_to_exec->description;
		success_handler(ZCLK_RES_SUCCESS, ZCLK_RESULT_STRING, help_str);
	}
	return ZCLK_RES_SUCCESS;
}

zclk_res get_help_for(char **help_str, arraylist *commands,
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
				zclk_command *cmd = arraylist_get(cmd_list, j);
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
				return ZCLK_RES_ERR_COMMAND_NOT_FOUND;
			}
		}
		else
		{
			return ZCLK_RES_ERR_COMMAND_NOT_FOUND;
		}
	}
	return ZCLK_RES_ERR_UNKNOWN;
}

zclk_res parse_options(arraylist *options, int *argc, char **argv)
{
	while (1)
	{
		zclk_option *found = NULL;
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
					zclk_option *opt = arraylist_get(options, j);
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
					snprintf(error_message_str, ZCLK_SIZE_OF_HELP_STR,
						"Unknown option %s.", option);
					return ZCLK_RES_ERR_OPTION_NOT_FOUND;
				}
				else
				{
					//printf("Found %s\n", found->name);
					skip_count += 1;
					//read option value if it is not a flag
					if (found->val->type == ZCLK_TYPE_FLAG)
					{
						zclk_val_set_bool(found->val, 1);
					}
					else
					{
						if (i == (*argc - 1))
						{
							snprintf(error_message_str, ZCLK_SIZE_OF_HELP_STR,
								"Value missing for option %s.", option);
							return ZCLK_RES_ERR_OPTION_NOT_FOUND;
						}
						else
						{
							skip_count += 1;
							char *value = argv[i + 1];
							parse_zclk_val(found->val, value);
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

	return ZCLK_RES_SUCCESS;
}

zclk_res parse_args(arraylist *args, int *argc, char **argv)
{
	size_t args_len = arraylist_length(args);
	// printf("args len %d\n", args_len);
	if (args_len <= *argc)
	{
		for (int i = 0; i < args_len; i++)
		{
			zclk_argument *arg = arraylist_get(args, i);
			char *argval = argv[i];
			//check if we have
			parse_zclk_val(arg->val, argval);
			// printf("Argval %s\n", argval);
		}
		for (int i = 0; i < args_len; i++)
		{
			*argc = gobble(*argc, argv, 0);
			// printf("new argc = %d, %p\n", *argc, argc);
		}
	}
	else
	{
		// snprintf(error_message_str, ZCLK_SIZE_OF_HELP_STR,
		// 	"Required arguments missing.");
		// return ZCLK_RES_ERR_ARG_NOT_FOUND;
		for (int i = 0; i < *argc; i++)
		{
			zclk_argument *arg = arraylist_get(args, i);
			char *argval = argv[i];
			//check if we have
			parse_zclk_val(arg->val, argval);
			//			printf("Argval %s\n", argval);
		}
		int tmp_argc = *argc;
		for (int i = 0; i < tmp_argc; i++)
		{
			*argc = gobble(*argc, argv, 0);
			// printf("new argc = %d\n", *argc);
		}
	}
	return ZCLK_RES_SUCCESS;
}

void print_options(arraylist *options)
{
	if (options != NULL)
	{
		size_t options_len = arraylist_length(options);
		for (int i = 0; i < options_len; i++)
		{
			zclk_option *o = arraylist_get(options, i);
			switch (o->val->type)
			{
			case ZCLK_TYPE_BOOLEAN:
				printf("Options%d %s, %s = %d\n", i, o->name, 
					o->short_name, zclk_val_get_bool(o->val));
				break;
			case ZCLK_TYPE_FLAG:
				printf("Options%d %s, %s = %d\n", i, o->name, 
					o->short_name, zclk_val_get_bool(o->val));
				break;
			case ZCLK_TYPE_STRING:
				printf("Options%d %s, %s = %s\n", i, o->name, 
					o->short_name, zclk_val_get_string(o->val));
				break;
			case ZCLK_TYPE_INT:
				printf("Options%d %s, %s = %d\n", i, o->name, 
					o->short_name, zclk_val_get_int(o->val));
				break;
			case ZCLK_TYPE_DOUBLE:
				printf("Options%d %s, %s = %g\n", i, o->name, 
					o->short_name, zclk_val_get_double(o->val));
				break;
			default:
				printf("Options%d %s, %s has unknown type\n", i, 
					o->name, o->short_name);
				break;
			}
		}
	}
}

zclk_res exec_command(arraylist *commands, void *handler_args,
						 int argc, char **argv)
{
	zclk_res err = ZCLK_RES_SUCCESS;

	//First read all commands
	arraylist *cmds_to_exec = get_command_to_exec(commands, &argc, argv);
	size_t len_cmds = arraylist_length(cmds_to_exec);
	arraylist *all_options, *all_args;
	arraylist_new(&all_options, NULL);
	#ifdef LUA_ENABLED
		set_lua_convertor(all_options, &arraylist_zclk_option_to_lua);
	#endif //LUA_ENABLED
	arraylist_new(&all_args, NULL);
	#ifdef LUA_ENABLED
		set_lua_convertor(all_args, &arraylist_zclk_argument_to_lua);
	#endif //LUA_ENABLED

	for (int i = 0; i < len_cmds; i++)
	{
		zclk_command *cmd_to_exec = arraylist_get(cmds_to_exec, i);
		size_t len_options = arraylist_length(cmd_to_exec->options);
		for (int j = 0; j < len_options; j++)
		{
			zclk_option *opt_to_add = arraylist_get(cmd_to_exec->options, j);
			size_t opt_len = arraylist_length(all_options);
			int opt_exists = 0;
			for (size_t k = 0; k < opt_len; k++)
			{
				zclk_option* opt_to_cmp = arraylist_get(all_options, k);
				if (strcmp(zclk_option_get_name(opt_to_add), 
						zclk_option_get_name(opt_to_cmp)) == 0
					&& strcmp(zclk_option_get_short_name(opt_to_add), 
						zclk_option_get_short_name(opt_to_cmp)) == 0)
				{
					opt_exists = 1;
					//printf("Option %s already exists.\n", zclk_option_get_name(opt_to_add));
				}
			}
			if (opt_exists == 0)
			{
				arraylist_add(all_options, opt_to_add);
			}
		}
		size_t len_args = arraylist_length(cmd_to_exec->args);
		for (int j = 0; j < len_args; j++)
		{
			arraylist_add(all_args, arraylist_get(cmd_to_exec->args, j));
		}
	}

	//print_args(argc, argv);

	//Then read all options
	err = parse_options(all_options, &argc, argv);
	if (err != ZCLK_RES_SUCCESS)
	{
		return err;
	}

	//print_options(all_options);

	zclk_option *help_option = get_option_by_name(all_options, ZCLK_OPTION_HELP_LONG);
	if (zclk_val_get_bool(help_option->val))
	{
		char *help_str = get_help_for_command(cmds_to_exec);
		if (help_str == NULL)
		{
			// print_handler(ZCLK_RES_ERR_COMMAND_NOT_FOUND, 
			//				ZCLK_RESULT_STRING,
			// 			  	"No valid command found. "\
			//				"Run again with --help to see usage.\n");

			snprintf(error_message_str, ZCLK_SIZE_OF_HELP_STR,
				"No valid sub-command found. Run main command with --help" \
				" for a list of available sub-commands.");

			return ZCLK_RES_ERR_COMMAND_NOT_FOUND;
		}
		print_handler(ZCLK_RES_SUCCESS, ZCLK_RESULT_STRING, help_str);
	}
	else
	{
		for (int i = 0; i < len_cmds; i++)
		{
			zclk_command *cmd_to_exec = arraylist_get(cmds_to_exec, i);
			if (cmd_to_exec == NULL)
			{
				snprintf(error_message_str, ZCLK_SIZE_OF_HELP_STR,
					"No valid sub-command found. Run main command with" \
					" --help for a list of available sub-commands.");
				return ZCLK_RES_ERR_COMMAND_NOT_FOUND;
			}

			// for the last command in the chain, it can have args
			if (i == (len_cmds - 1))
			{
				//Now read all arguments
				err = parse_args(cmd_to_exec->args, &argc, argv);
				if (err != ZCLK_RES_SUCCESS)
				{
					return err;
				}

				//anything leftover
				if (argc > 0)
				{
					snprintf(error_message_str, ZCLK_SIZE_OF_HELP_STR,
						"%d extra arguments found.\n", argc);
					return ZCLK_RES_ERR_EXTRA_ARGS_FOUND;
				}
			}

			if (cmd_to_exec->handler != NULL)
			{
				err = cmd_to_exec->handler(cmd_to_exec, handler_args);
			}
		}
	}

	arraylist_free(cmds_to_exec);
	arraylist_free(all_options);
	arraylist_free(all_args);
	return err;
}

void print_table_result(void* result)
{
	zclk_table* result_tbl = (zclk_table*)result;
	size_t* col_widths;
	col_widths = (size_t*)calloc(result_tbl->num_cols, sizeof(size_t));
	char** col_fmtspec;
	col_fmtspec = (char**)calloc(result_tbl->num_cols, sizeof(char*));
	char* header;
	char* value;
	int min_width = 4, max_width = 25;

	//calculate column widths, and create format specifiers
	for (int i = 0; i < result_tbl->num_cols; i++)
	{
		zclk_table_get_header(&header, result_tbl, i);
		size_t col_width = strlen(header);
		for (int j = 0; j < result_tbl->num_rows; j++)
		{
			zclk_table_get_row_val(&value, result_tbl, j, i);
			if (value != NULL)
			{
				if (strlen(value) > col_width)
				{
					col_width = strlen(value);
				}
			}
		}
		if (col_width < min_width)
		{
			col_width = min_width;
		}
		if (col_width > max_width)
		{
			col_width = max_width;
		}
		char* fmtspec = (char*)calloc(16, sizeof(char));
		if (fmtspec != NULL)
		{
			sprintf(fmtspec, "%%-%zu.%zus", (col_width + 1), col_width);
			col_widths[i] = col_width;
			col_fmtspec[i] = fmtspec;
		}
		else
		{
			col_fmtspec[i] = "fatal error creating format specifiers";
		}
	}

	printf("\n");
	for (int i = 0; i < result_tbl->num_cols; i++)
	{
		zclk_table_get_header(&header, result_tbl, i);
		printf(col_fmtspec[i], header);
	}
	printf("\n");
	for (int i = 0; i < result_tbl->num_cols; i++)
	{
		for (int j = 0; j < col_widths[i] + 1; j++)
		{
			printf("-");
		}
	}
	printf("\n");

	for (int i = 0; i < result_tbl->num_rows; i++)
	{
		for (int j = 0; j < result_tbl->num_cols; j++)
		{
			zclk_table_get_row_val(&value, result_tbl, i, j);
			if (value == NULL)
			{
				printf(col_fmtspec[j], "");
			}
			else
			{
				printf(col_fmtspec[j], value);
			}
		}
		printf("\n");
	}
	printf("\n");

	for (int i = 0; i < result_tbl->num_cols; i++)
	{
		free(col_fmtspec[i]);
	}
}

zclk_res print_handler(zclk_res result_flag, zclk_result_type res_type,
	void* result)
{
	if (res_type == ZCLK_RESULT_STRING)
	{
		if (result != NULL)
		{
			char* result_str = (char*)result;
			printf("%s", result_str);
		}
	}
	else if (res_type == ZCLK_RESULT_TABLE)
	{
		print_table_result(result);
	}
	else if (res_type == ZCLK_RESULT_DICT)
	{
		zclk_dict* result_dict = (zclk_dict*)result;
		zclk_dict_foreach(result_dict, k, v)
		{
			printf("%-26.25s: %s\n", k, v);
		}
		printf("\n");
	}
	else if (res_type == ZCLK_RESULT_PROGRESS)
	{
		zclk_multi_progress* result_progress = (zclk_multi_progress*)result;
		if (result_progress->old_count > 0)
		{
			printf("\033[%dA", result_progress->old_count);
			fflush(stdout);
		}
		size_t new_len = arraylist_length(result_progress->progress_ls);
		//		printf("To remove %d, to write %d\n", result_progress->old_count, new_len);
		for (size_t i = 0; i < new_len; i++)
		{
			zclk_progress* p = (zclk_progress*)arraylist_get(
				result_progress->progress_ls, i);
			printf("\033[K%s: %s", p->name, p->message);
			char* progress = p->extra;
			if (progress != NULL)
			{
				printf(" %s", progress);
			}
			printf("\n");
		}
	}
	else
	{
		printf("This result type is not handled %d\n", res_type);
	}
	return ZCLK_RES_SUCCESS;
}
