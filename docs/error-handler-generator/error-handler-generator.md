# Error handler generator

This small [ruby](https://www.ruby-lang.org/en/) project is simple tool
which can be used to create a **C library** that can manage critical errors
inside embedded systems.

The folder structure is as follow:

- `bin`: the folder that contains the executables
    - `error_gen`: the main executable
- `examples`: a folder that contains an example of input and generated code
- `lib`: the folder containing the actual code used by the main executable
    - `error.rb`: a class representing a single [error group](error.md)
    - `error_gen.rb`: the generator class
    - `templates.rb`: the templates used to generate code
    - `version.rb`: the current version of the library
    - `master_logger.rb`: just a static wrapper to the ruby logger class
- `error_gen.gemspec`: file used by gem to build and install this project

## Parsing

Once the executable is run with the right parameters, the json file given as input
is parsed gathering all the necessary data.

!!! tip
    For info about the executable usage and/or options you can type `error_gen -h` or `error_gen --help`

After the parsing is completed the source and header files are created in the **same
folder** as the input file and the templates are written filling out all the required data.

