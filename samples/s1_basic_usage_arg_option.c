#include <zclk.h>
#include <stdio.h>

zclk_cmd_err basic_handler(void* handler_args,
	arraylist* options, arraylist* args,
	zclk_command_output_handler success_handler,
	zclk_command_output_handler error_handler)
{
    for (size_t i = 0; i < arraylist_length(options);i++)
    {
        zclk_option *opt = (zclk_option*)arraylist_get(options, i);
        printf("option: %s=%d\n", opt->name, zclk_val_get_bool(opt->val));
    }
    return 0;
}

int main(int argc, char* argv[])
{
    printf("Sample#1: Basic Usage of CLIUTILS with "
        "One Argument and One Option.\n");

    zclk_command *cmd = new_zclk_command(argv[0], "cmd",
                            "Basic Description", &basic_handler);
    
    zclk_command_option_add(
        cmd,
        new_zclk_option(
            "option-one",
			"o", 
            zclk_flag(0),
            zclk_flag(0), 
            "Option One"));

    zclk_command_argument_add(
        cmd,
        new_zclk_argument(
            "arg-one",
            zclk_string("blah"),
            zclk_string("blah"),
            "Argument One"));
    
    zclk_command_exec(cmd, argc, argv);

    printf("Created app successfully!\n");
}
