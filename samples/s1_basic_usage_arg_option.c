#include <zclk.h>
#include <stdio.h>

zclk_res basic_handler(zclk_command* cmd, void* handler_args)
{
    printf("Sample#1: Basic Usage of CLIUTILS with "
        "One Argument and One Option.\n");

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

    return 0;
}

int main(int argc, char* argv[])
{
    zclk_command *cmd = new_zclk_command(argv[0], "cmd",
                            "Basic Description", &basic_handler);
    
    zclk_command_flag_option(
        cmd,
        "option-one",
        "o", 
        ZCLK_FLAG_OFF, 
        "Option One"
    );

    zclk_command_string_argument(
        cmd,
        "arg-one",
        "blah",
        "Argument One",
        1
    );
    
    zclk_command_exec(cmd, NULL, argc, argv);
}
