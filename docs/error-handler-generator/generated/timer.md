# Timer

All the timings are not managed internally by the library but the implementation
of a timing handler is left to the user.

In embedded systems, **timers** are often used to manage timings precisely to achieve
the desired behaviour, that's why in the library the timer is often referenced even if
the timings handler is something else.

A **single timer** is used, because there is no need to keep track simultaneously of multiple
errors, just the one that should expire first.

If more than one error expires in the same moment as the first, it is removed as well at the
same time. 

