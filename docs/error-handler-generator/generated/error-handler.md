# Error handler library

The generated library handles all the defined errors and the operations that can be done with them.

## Concept

When a critical problem is encountered in an embedded system, if the problem persist
for a certain amount of time, the appropiate safety maesures should be taken.

To notify the problem to the system, the corresponding error is set until a certian amount of
time elapses and the error is considered as expired.

The lifetime of an error start when it is set and ends after it is reset or expired,
the main difference between the last two are that if the error is reset it can be set again
othwerise it has to be considered critical and all the system has to be restored to the inital states.

