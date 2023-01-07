#include <zclk.h>
#include <stdio.h>

zclk_cmd_err basic_handler(void* handler_args,
	arraylist* options, arraylist* args,
	zclk_command_output_handler success_handler,
	zclk_command_output_handler error_handler)
{
    printf("***************************************"
        "****************************\n");
    printf("Sample#1: Basic Usage of CLIUTILS with "
        "One Argument and One Option.\n");
    printf("***************************************"
        "****************************\n");

    printf("\n** Options\n");
    for (size_t i = 0; i < arraylist_length(options);i++)
    {
        zclk_option *opt = (zclk_option*)arraylist_get(options, i);
        printf("\toption: %s=%d\n", opt->name, zclk_val_get_bool(opt->val));
    }

    printf("***************************************"
        "****************************\n");
    return 0;
}

int main(int argc, char* argv[])
{
    zclk_command *cmd = new_zclk_command(argv[0], "cmd",
                            "Basic Description", &basic_handler);
    
    zclk_command_option_add(
        cmd,
        new_zclk_option_flag(
            "option-one",
			"o", 
            ZCLK_FLAG_OFF,
            ZCLK_FLAG_OFF, 
            "Option One"));

    zclk_command_argument_add(
        cmd,
        new_zclk_argument_string(
            "arg-one",
            "blah",
            "blah",
            "Argument One"));
    
    zclk_command_exec(cmd, argc, argv);
}
