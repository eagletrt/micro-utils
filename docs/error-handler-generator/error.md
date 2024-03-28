# Error

When a problem, which can be considered critical, happens inside a system, an **error**
can be used to store information about that specific problem.

A single error can be identified by:

- The **group** to which it belongs
- Its **instance** inside the group

An error has three different possible states:

1. **Unset**: if the corresponding problem is not present (hence there is no error)
2. **Set**: if the problem happened
3. **Expired**: if the problem persists after a certain amount of time

An error cannot be considered expired immediately after being set due to
*false positives* where the problem occurs (or so it seems) for a very short period of time.

The amount of time required before the error expire is called `timeout`.

