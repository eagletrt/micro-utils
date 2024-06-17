# Change Log

## [1.6.1] - 17-06-2024

### Fixed

- Running groups counter decremented inside the expire immediate function even if
the error was not running

## [1.6.0] - 17-06-2024

### Added

- Better console logging in case of errors, warnings or info
- Custom optional description for each error group
- Optional enum aliases for the error groups instances

### Fixed

- Break instead of return inside _error_expire_immediate function

## [1.5.1] - 16-06-2024

### Refactor

- Template code refactoring

## [1.5.0] - 13-06-2024

### Added

- Add function to expire the errors immediately without setting them first

### Fixed

- Running groups counter not decremented when error expires

## [1.4.0] - 05-06-2024

### Added

- Option to add a prefix to the generated types, macros and functions

## [1.3.4] - 04-06-2024

### Fixed

- Change ssize_t type to signed_size_t

## [1.3.3] - 15-04-2024

### Fixed

- Expired errors buffer not updating correctly

## [1.3.2] - 14-04-2024

### Chore

- Update version of the ring buffer and the min heap libraries

## [1.3.1] - 28-03-2024

### Added

- Dependencies list inside README.md

### Fixed

- Empty filename in the 'generated from' banner field at the top of the generated source
and header files

## [1.3.0] - 24-03-2024

### Added

- Function that returns the current number of running errors for a single group
- Function that returns the current number of expired errors for a single group
- Function that copies all the running errors into an array
- Function that copies all the expired errors into an array
- Function that copies all the running groups into an array
- Function that copies all the expired groups into an array

### Changed

- Changed wrong filenames in README.md build and install commands

## [1.2.0] - 23-03-2024

### Added

- Function that returns the current number of running errors
- Function that returns the current number of expired errors

## [1.1.0] - 22-03-2024

### Added

- Macro to set or reset an error based on a given boolean condition

### Changed

- Give critical section handler functions as parameters to the initializer function
  instead of defining them with the specific names (required by the new version of the ring buffer library)


