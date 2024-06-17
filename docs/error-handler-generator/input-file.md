# Input file

The file that should be given as input to the generator is in Json format, this is due
to its simplicity and extensive support as a well known file format.

The file can have any name but it must end with the `.json` extension.

The json file contains a single element named `errors`, that **must not be modified**,
which is a list of objects that defines a group of errors.

## Mandatory parameters

A single error group requires the following fields to be generated correctly:

- `name`: the name of the group (preferably all **upper case**, with words separated by an **underscore** (e.g. `FERIE`)
- `timeout`: the amount of time that should elapse before the error can be considered expired
- `instances`: the total number of instances of that group

!!! note
    If any of the required fields of a group are missing the object is ignored and
    the corresponding data is not generated.

## Optional parameters

Some optional parameters can be added to the input file for documentation or utility
purposes, for example:

- `description`: a small description of the error and what it represent
- `details`: in-depth info about each error instance

The details is a list of objects that contains aliases which can be used to identify
a particular instance of an error without having to remember the actual index.

To specify an alias two fields are required:

- `id`: the index of the instance
- `alias`: the alias to associate with the instance index

!!! note
    If either the id or the alias are missing, an id is repeated mutliple times
    or is not valid (i.e. is less than 0 or greater than the total number of instances)
    the instance details is ignored and its corresponding code is not generated

!!! attention
    The generator does not check for multiple aliases with the same name within
    a single error group
