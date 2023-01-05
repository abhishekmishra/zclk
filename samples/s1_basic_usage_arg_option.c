#include <cliutils.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    printf("Sample#1: Basic Usage of CLIUTILS with One Argument and One Option.\n");

    zclk_command *cmd = zclk_command_new("command", "cmd", "Basic Description", NULL);
    printf("Created app successfully!\n");
}