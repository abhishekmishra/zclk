#include <cliutils.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    printf("Sample#1: Basic Usage of CLIUTILS with One Argument and One Option.\n");

    cli_cmd_err errcode;
    cli_app *app;
    errcode = create_cli_app(&app, "Basic App", "Basic Description");

    if(errcode == CLI_COMMAND_SUCCESS)
    {
        printf("Created app successfully!\n");
    }
}