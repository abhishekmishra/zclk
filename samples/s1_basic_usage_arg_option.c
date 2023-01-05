#include <cliutils.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    printf("Sample#1: Basic Usage of CLIUTILS with "
        "One Argument and One Option.\n");

    zclk_command *cmd = zclk_command_new(argv[0], "cmd",
                            "Basic Description", NULL);

    zclk_command_exec(cmd, argc, argv);

    printf("Created app successfully!\n");
}
