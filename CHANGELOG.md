# CHANGELOG
This file documents major changes in every release of the project. The project
follows [Semantic Versioning](https://semver.org/). There is a section for each
release - which lists major changes made in the release.

**0.1.0-alpha.3**  2023-02-23 Abhishek Mishra  <abhishekmishra3@gmail.com>

- Bug fix in argument parsing. Arguments are parsed from left to right and
  missing args are given default values. It is the responsibility of the
  program to deal with missing arguments (assigned to default values).
  If there are more arguments passed than the command requires, an error is
  displayed and program fails.

**0.1.0-alpha.2**  2023-02-21 Abhishek Mishra  <abhishekmishra3@gmail.com>

- This is the 3rd alpha release. It incorporates one major feature and some
  minor API changes and bug fixes.
- Added lua module implementation for zclk, along with some sample programs.
- Minor api changes to add const qualifier where it makes sense.
- The flag and boolean options don't need a default value as it is always
  false.

**0.1.0-alpha.1**  2023-01-21 Abhishek Mishra  <abhishekmishra3@gmail.com>

- This is the second alpha release. It incorporates major changes in API, and 
  a few minor improvements in error handling.
- All public API functions that accept strings now use `const char *` instead 
  of `char *`. All strings are cloned internally before usage.
- New functions added to retrieve options and arguments from the command object
  using their names. This is more convenient and produces more readable code 
  than looping over the options and arguments and conditional operation over 
  the names.
- The public APIs creating options and arguments is fixed to accept only 
  `default_val`. The `val` property of the corresponding options or argument is
   set initially to `default_val`.
- Some error messages are added for common errors in parsing options and 
  arguments. This mechanism might change in a future version.
- A few minor build issues, compiler warnings, and a logic error were fixed.

**0.1.0-alpha.0**  2023-01-14 Abhishek Mishra  <abhishekmishra3@gmail.com>

- The `ZClk` library came out of my efforts to create a Docker CLI client
  similar to the official **Docker CLI**. I needed to support commands, sub-
  command, options, arguments and values of different types. I also wanted to
  implement some of the commands entirely in lua if possible.
- This first release contains all the features required - all though it is an
  early release and require polish and refactoring in many areas.
- The [CLD: Docker CLI](https://github.com/abhishekmishra/CLD) project uses
  this library to provide functionality similar to the _official_ Docker CLI.
- The library C API documentation is generated using doxygen and is available
  both online and can be generated offline using the `make docs` command.
- The `./samples` folder contains sample programs showing how to use the API.
  More samples will be added in the coming releases.