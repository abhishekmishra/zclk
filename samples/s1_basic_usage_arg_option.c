#include <zclk.h>
#include <stdio.h>

zclk_cmd_err basic_handler(zclk_command* cmd, void* handler_args)
{
    printf("***************************************"
        "****************************\n");
    printf("Sample#1: Basic Usage of CLIUTILS with "
        "One Argument and One Option.\n");
    printf("***************************************"
        "****************************\n");

    printf("\n** Options\n");

    zclk_command_option_foreach(cmd, opt)
    {
        printf("\toption: %s=%d\n", opt->name, zclk_option_get_val_flag(opt));
    }

    printf("\n** Arguments\n");

    zclk_command_argument_foreach(cmd, argument)
    {
        printf("\targument: %s=%s\n", argument->name, zclk_argument_get_val_string(argument));
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
    
    zclk_command_exec(cmd, NULL, argc, argv);
}
