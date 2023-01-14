# CHANGELOG
This file documents major changes in every release of the project. The project follows [Semantic Versioning](https://semver.org/). There is a section for each release - which lists major changes made in the release.

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