# Operations

The majority of the operations that can be performed are access operations that
does not modify the internal data of the error handler library, except three main operations.

## Modifiers

The three main operations that modifies the internal data are:

1. The [set](#set) operation
2. The [reset](#reset) operation
3. The [expire](#expire) operation

!!! note
    Those operations can be executed only on one error instance at a time

### Set

The **set** operation requires the group and instance of the error to set, as well as
the current time to update the error correctly.

If the parameters are valid and the error is not already running or it is expired,
its information are updated and the error is inserted inside the heap and,
if the error should be the first to expire, the update timer callback is called.

### Reset

The **reset** operation requires the group and instance of the error to reset.

If the parameters are valid and the error is running and it has not already expired,
its information are updated and the error is removed from the heap.

If the removed errors was the first to expire the update timer callback is called
if the heap is not empty (that means that other errors are still running), otherwise
there are no more errors hence the stop timer callback is called.

### Toggle if

There is a fourth operation, which is the **toggle if**, that's just a shortcut
to set or reset an error based on a condition, this can be useful in a variety of cases.

### Expire

The **expire** operation does not require any parameters because it gets all the information
from the first element inside the heap.

If the error is running and is not expired yet, until an error that has not already
expired is incountered or the are no more errors, the information of the error are updated,
the error is added to the buffer of expired errors and removed from the heap, then
the update timer callback is called if there is at least one error still running, otherwise
the stop timer callback is called.

### Routine

The error routine is one of the most important function of all, its purpose is to
get the data from the buffer, if it's not empty, and execute the appropriate function
on the gathered data.

The main purpose of this function is to handle all the operations that were started
but still have to be executed.

!!! warning
    The routine function should be called as often as possible to avoid possible loss
    of data

## Access

There are a number of operations that can be used to gather information about the
internal state of the error handler and the errors themselves.

The `get_running` and `get_expired` functions returns the current number of running and
expired errors respectively, and the `get_group` version of them returns the number of running
and expired errors for a specific group.

The `dump_running` and `dump_expired` functions returns a copy of all the errors
that are currently running or that are expired, the `groups` version of them
returns a copy of the groups in which at least one error instance is running or expired.

## Callbacks

A few callbacks functions are defined inside the library that should be redefined by
the user.

The `update_timer` callback function should update the timings handler (usually a timer)
givent the time in which the error was set and its corresponding timeout.

This function is called every time a new error expires before the one that's currently handled
or when the first error that should expire is reset.

The `stop_timer` callback is used to stop the timings handler in case there are no
errors set.

