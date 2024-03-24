# Change Log

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


