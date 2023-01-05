#include <cliutils.h>
#include <stdio.h>

cli_cmd_err cmd_one_handler(void* handler_args,
	arraylist* options, arraylist* args,
	cli_command_output_handler success_handler,
	cli_command_output_handler error_handler)
{
    printf("Handler\n");
}

int main(int argc, char* argv[])
{
    printf("Sample#1: Basic Usage of CLIUTILS with One Argument and One Option.\n");

    cli_option *opt_one;
    cli_argument *arg_one;
    cli_command *cmd_one;
    cli_cmd_err errcode;

    errcode = make_command(
                &cmd_one, 
                "cmd_one", "c1", 
                "A basic first command called cmd_one", 
                &cmd_one_handler
            );

    if(errcode == CLI_COMMAND_SUCCESS)
    {
        printf("Created command successfully!\n");
    }
}