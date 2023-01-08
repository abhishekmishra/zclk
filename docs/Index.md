# Zclk Documentation                {#mainpage}

**NOTE: This library is still under development. The public API is unstable till the library reaches beta status. There will be breaking changes in public API if needed.**

**ZCLK** is a C library to build command line interfaces aka CLI programs.

# Getting Started

## Build the package using CMake
The Makefile in the source folder contains targets for useful CMake commands.
You can use the genbuild and build targets to create the CMake build folder,
and then run the build for your platform.

```bash
$ make genbuild build
```
## Use the library in your code.
A library named zclk (with appropriate prefix/suffix for your platform) will
be generated in the ./build folder.

Use this library and add the src folder to your include folders during
compilation.

## Simple Example
The `zclk_command` struct and its associated functions provide the majority
of the public api of the library. There are functions to create commands,
add options and arguments and then execute the commands given the program
arguments. These are all demostrated in sample programs in the `./samples`
folder in the source code.

Here is a simple implementation with one command which has one option and one
argument - to give a feel of the API...

```c
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
    
    zclk_command_flag_option(
        cmd,
        "option-one",
        "o", 
        ZCLK_FLAG_OFF,
        ZCLK_FLAG_OFF, 
        "Option One"
    );

    zclk_command_string_argument(
        cmd,
        "arg-one",
        "blah",
        "blah",
        "Argument One",
        1
    );
    
    zclk_command_exec(cmd, NULL, argc, argv);
}

```

# Public API Header
The public API of the library is provided by [zclk.h](@ref zclk.h).