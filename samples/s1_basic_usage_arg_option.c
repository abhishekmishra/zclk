#include <cliutils.h>
#include <stdio.h>

cli_cmd_err basic_handler(void* handler_args,
	arraylist* options, arraylist* args,
	zclk_command_output_handler success_handler,
	zclk_command_output_handler error_handler)
{
    for (size_t i = 0; i < arraylist_length(options);i++)
    {
        cli_option *opt = (cli_option*)arraylist_get(options, i);
        printf("option: %s=%d\n", opt->name, opt->val->bool_value);
    }
}

int main(int argc, char* argv[])
{
    printf("Sample#1: Basic Usage of CLIUTILS with "
        "One Argument and One Option.\n");

    zclk_command *cmd = zclk_command_new(argv[0], "cmd",
                            "Basic Description", &basic_handler);
    
    zclk_command_option_add(
        cmd,
        create_option(
            "option-one",
			"o", 
            CLI_VAL_FLAG(0),
            CLI_VAL_FLAG(0), 
            "Option One"));

    zclk_command_argument_add(
        cmd,
        create_argument(
            "arg-one",
            CLI_VAL_STRING("blah"),
            CLI_VAL_STRING("blah"),
            "Argument One"));
    
    zclk_command_exec(cmd, argc, argv);

    printf("Created app successfully!\n");
}
