# ERROR-GEN

This small project, written entirely in [Ruby](https://www.ruby-lang.org/it/), is a
code generator utility that can take as an input a *Json* file with the definition of a
list of critical errors and give as output the **C code** that handles them.

> [!NOTE]
> Examples can be found inside the example folder

## Dependencies

The generated library depends of the following libraries, that can be found on the [micro-libs](https://github.com/eagletrt/micro-libs) repository:
- [ring-buffer](https://github.com/eagletrt/micro-libs/tree/master/ring-buffer)
- [min-heap](https://github.com/eagletrt/micro-libs/tree/master/min-heap)

## Concept

The user can define groups of errors that specify a certain category to which the single error
belongs to, like for example under voltages, over currents, and more...

Each group of errors can have many independent instances; this is needed because if two errors
belongs to the same group, without instances, they are treated as the same error and if one is
reset the other gets reset as well even if the error is still present.

When an error is *set* it means that the error is in the **running** state but is
not expired yet, in this state the error can be *reset* removing it from the list of
running errors.

If enough time passes when an error is in the **running** state it switch to the **expired**
state, where the error can be no longer *reset* and can be treated as *critical*

> [!IMPORTANT]
> Time management has to be done outside the generated library, by the user, with a timer for example

## Errors definition

The Json file has the following format:
```json
{
    "errors": [
        {
            "name": "ERROR_NAME",
            "timeout": 100,
            "instances": 10
        },
        ...
    ]
}
```

The `errors` array contains the list of errors and each error is defined by:
1. `name`: The name of the error group in uppercase with (or without) underscores
2. `timeout`: How much time should elapse (in ms) before the error expire (to avoid false positives)
3. `instances`: The number of error instances for that group

## Usage

To use this library first you have to install [Gem](https://rubygems.org/), the ruby
package manager.

The go to the root folder of this project (where the `error_gen.gemspec` file is located),
build and install the gem:
```shell
gem build -o error_gen.gem   # Build
gem install error_gen.gem    # Install
```

> [!WARNING]
> For unix-like systems sudo privileges might be required

Then you can run the binary in the `bin` folder to generate the code given the input file:
```shell
./bin/error_gen examples/simple.json
```

> [!NOTE]
> The generated C source files (`.c/.h`) is located in the same folder as the input Json file

