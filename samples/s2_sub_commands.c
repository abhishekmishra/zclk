#include <zclk.h>
#include <stdio.h>

zclk_cmd_err main_command(zclk_command* cmd, void* handler_args)
{
    printf("Main Command.\n");
    return 0;
}

zclk_cmd_err sub_command(zclk_command* cmd, void* handler_args)
{
    printf("Sub Command.\n");
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
    /* Create a new command with main_command as handler*/
    zclk_command *main_cmd = new_zclk_command(argv[0], "cmd",
                            "Main Command", &main_command);

    /* Create a new command with sub_command as handler */
    zclk_command *sub_cmd = new_zclk_command("subcommand", "sub", 
    "Sub Command",&sub_command);

    /* Add an option to the sub command */
    zclk_command_flag_option(
        sub_cmd,
        "option-one",
        "o", 
        ZCLK_FLAG_OFF,
        ZCLK_FLAG_OFF, 
        "Option One"
    );

    /* Add the sub command to the main command */
    zclk_command_subcommand_add(main_cmd, sub_cmd);

    /* Execute the main command with command line args */
    return zclk_command_exec(main_cmd, NULL, argc, argv);
}
