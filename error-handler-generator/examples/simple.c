/*******************************************************************************
 * Critical error handler library generator
 * Generated by error_gen ruby gem, for more information see:
 * https://github.com/eagletrt/micro-utils/tree/master/error-handler-generator
 *
 * Error_gen version 1.6.0
 * Generation date: 2024-06-17 03:09:03 +0200
 * Generated from: examples/simple.json
 * With prefix: none
 * The error handler contains:
 *     - 3 error groups
 *     - 119 total error instances
 ******************************************************************************/

#include "simple.h"

#include <string.h>

#include "ring-buffer.h"
#include "min-heap.h"

// Ring buffer maximum number of elements
#define ERROR_BUFFER_SIZE 16

/**
 * @brief Error data type definition needed to manage the errors
 *
 * @param group The group to which the error belongs
 * @param instance The instance of the error
 * @param timestamp The current time (in ms)
 * @param op A pointer to the operation that needs to be executed
 */
typedef struct _ErrorData {
    ErrorGroup group;
    ErrorInstance instance;
    uint32_t timestamp;
    void (* op)(struct _ErrorData);
} ErrorData;

/** @brief Total number of instances for each group */
static const uint16_t instances[] = {
    [ERROR_GROUP_FIRST] = 10,
    [ERROR_GROUP_SECOND] = 108,
    [ERROR_GROUP_THIRD] = 1
};
/** @brief Error timeout for each group */
static const uint16_t timeouts[] = {
    [ERROR_GROUP_FIRST] = 100,
    [ERROR_GROUP_SECOND] = 250,
    [ERROR_GROUP_THIRD] = 2000
};

/** @brief List of errors where the data is stored */
static Error error_first_instances[10];
static Error error_second_instances[108];
static Error error_third_instances[1];
static Error * errors[] = {    
    [ERROR_GROUP_FIRST] = error_first_instances,
    [ERROR_GROUP_SECOND] = error_second_instances,
    [ERROR_GROUP_THIRD] = error_third_instances
};

/** @brief Function declaration needed for the min heap */
int8_t _error_compare(void * a, void * b);

bool routine_lock = false;
RingBuffer(ErrorData, ERROR_BUFFER_SIZE) err_buf;
RingBuffer(Error *, ERROR_INSTANCE_COUNT) expired_errors = ring_buffer_new(Error *, ERROR_INSTANCE_COUNT, NULL, NULL);
MinHeap(Error *, ERROR_INSTANCE_COUNT) running_errors = min_heap_new(Error *, ERROR_INSTANCE_COUNT, _error_compare);

/** @brief Fast lookup for groups that are running or expired */
uint16_t running_groups[ERROR_GROUP_COUNT];
uint16_t expired_groups[ERROR_GROUP_COUNT];

/**
 * @brief Compare two errors based on the time when they were set
 * and their timeouts
 *
 * @param t1 The timestamp of the first error
 * @param dt1 The timeout of the first error
 * @param t2 The timestamp of the second error
 * @param dt2 The timeout of the second error
 * @return int32_t The difference between the two expire times
 */
int8_t _error_compare(void * a, void * b) {
    Error * e1 = *(Error **)a;
    Error * e2 = *(Error **)b;
    int32_t t1 = e1->timestamp + timeouts[e1->group];
    int32_t t2 = e2->timestamp + timeouts[e2->group];
    return t1 < t2 ? -1 : (t1 == t2 ? 0 : 1);
}

/**
 * @brief Set the error if possible and update the timer if necessary
 *
 * @param data The data of the error to set
 */
void _error_set(ErrorData data) {
    // Get error
    Error * err = &errors[data.group][data.instance];
    if (err->is_running || err->is_expired)
        return;

    // Update error info
    err->is_running = true;
    err->timestamp = data.timestamp;
    ++running_groups[data.group];

    // Add error to the running list of errors and
    // update timer if the error is the first to expire
    if (min_heap_insert(&running_errors, &err) != MIN_HEAP_OK)
        return;
    Error ** top = (Error **)min_heap_peek(&running_errors);
    if (top != NULL && *top == err)
        error_update_timer_callback(err->timestamp, timeouts[err->group]);
}

/**
 * @brief Reset the error if possible and update the timer if necessary
 *
 * @param data The data of the error to reset
 */
void _error_reset(ErrorData data) {
    // Get error
    Error * err = &errors[data.group][data.instance];
    if (!err->is_running || err->is_expired)
        return;

    // Update error info
    err->is_running = false;
    --running_groups[data.group];

    // Get the current first element
    Error * top = NULL;
    if (min_heap_top(&running_errors, &top) != MIN_HEAP_OK)
        return;

    if (top == err) {
        // If the removed error is the first in the heap
        // remove it and update (or stop) the timer
        if (min_heap_remove(&running_errors, 0, NULL) != MIN_HEAP_OK)
            return;

        if (min_heap_is_empty(&running_errors))
            error_stop_timer_callback();
        else if (min_heap_top(&running_errors, &top) == MIN_HEAP_OK)
            error_update_timer_callback(top->timestamp, timeouts[top->group]);
    }
    else {
        // Find and remove the error
        signed_size_t i = min_heap_find(&running_errors, &err);
        if (i < 0) return;
        min_heap_remove(&running_errors, i, NULL);
    }
}

/**
 * @brief Expire the error
 *
 * @param data The data of the error to expire
 */
void _error_expire(ErrorData data) {
    // Get error
    Error * top = NULL;
    if (min_heap_top(&running_errors, &top) != MIN_HEAP_OK)
        return;

    if (!top->is_running || top->is_expired)
        return;

    Error * prev = top;
    do {
        // Update error info
        top->is_running = false;
        top->is_expired = true;
        --running_groups[data.group];
        ++expired_groups[data.group];

        // Add error to the list of expired errors
        if (ring_buffer_push_back(&expired_errors, &top) != RING_BUFFER_OK)
            break;

        // Get next error and remove the previous
        if (min_heap_remove(&running_errors, 0, NULL) != MIN_HEAP_OK)
            break;

        // Stop the timer if there are no more errors
        if (min_heap_is_empty(&running_errors)) {
            error_stop_timer_callback();
            return;
        }

        // Get next errors
        if (min_heap_top(&running_errors, &top) != MIN_HEAP_OK)
            break;
    } while(_error_compare(&top, &prev) <= 0);

    // Update the timer
    if (top != NULL)
        error_update_timer_callback(top->timestamp, timeouts[top->group]);
}
/**
 * @brief Expire the error immediately without waiting for the timer
 *
 * @param data The data of the error to expire
 */
void _error_expire_immediate(ErrorData data) {
    // Get error
    Error * err = &errors[data.group][data.instance];
    if (err->is_expired)
        return;

    // Update error info
    err->is_running = false;
    err->is_expired = true;
    --running_groups[data.group];
    ++expired_groups[data.group];

    // Add error to the list of expired errors
    if (ring_buffer_push_back(&expired_errors, &err) != RING_BUFFER_OK)
        return;

    // Remove the error from the list of running errors
    signed_size_t index = min_heap_find(&running_errors, &err);
    if (index < 0)
        return;
    if (min_heap_remove(&running_errors, index, NULL) != MIN_HEAP_OK)
        return;

    // Stop the timer if there are no more errors
    if (min_heap_is_empty(&running_errors)) {
        error_stop_timer_callback();
        return;
    }
    else if (index == 0) {
        // Update the timer with the next error data
        Error ** next = (Error **)min_heap_peek(&running_errors);
        if (next != NULL)
            error_update_timer_callback((*next)->timestamp, timeouts[(*next)->group]);
    }
}

void error_init(void (* cs_enter)(void), void (* cs_exit)(void)) {
    ring_buffer_init(&err_buf, ErrorData, ERROR_BUFFER_SIZE, cs_enter, cs_exit);

    for (size_t i = 0; i < instances[ERROR_GROUP_FIRST]; ++i)
        error_first_instances[i].group = ERROR_GROUP_FIRST;
    for (size_t i = 0; i < instances[ERROR_GROUP_SECOND]; ++i)
        error_second_instances[i].group = ERROR_GROUP_SECOND;
    for (size_t i = 0; i < instances[ERROR_GROUP_THIRD]; ++i)
        error_third_instances[i].group = ERROR_GROUP_THIRD;
}
size_t error_get_running(void) {
    return min_heap_size(&running_errors);
}
size_t error_get_expired(void) {
    return ring_buffer_size(&expired_errors);
}
uint16_t error_get_group_running(ErrorGroup group) {
    if (group >= ERROR_GROUP_COUNT)
        return 0U;
    return running_groups[group];
}
uint16_t error_get_group_expired(ErrorGroup group) {
    if (group >= ERROR_GROUP_COUNT)
        return 0U;
    return expired_groups[group];
}
size_t error_dump_running(Error * out) {
    if (out == NULL)
        return 0U;

    err_buf.cs_enter();
    // Copy data
    size_t i;
    for (i = 0; i < running_errors.size; ++i)
        memcpy(&out[i], running_errors.data[i], sizeof(out[0U]));
    err_buf.cs_exit();
    return i;
}
size_t error_dump_expired(Error * out) {
    if (out == NULL)
        return 0U;

    err_buf.cs_enter();
    // Copy data
    size_t i;
    for (i = 0; i < expired_errors.size; ++i)
        memcpy(&out[i], expired_errors.data[i], sizeof(out[0U]));
    err_buf.cs_exit();
    return i;
}
size_t error_dump_running_groups(ErrorGroup * out) {
    if (out == NULL)
        return 0U;
    // Copy data
    size_t cnt = 0;
    for (size_t i = 0; i < ERROR_GROUP_COUNT; ++i)
        if (running_groups[i] > 0)
            out[cnt++] = i;
    return cnt;
}
size_t error_dump_expired_groups(ErrorGroup * out) {
    if (out == NULL)
        return 0U;
    // Copy data
    size_t cnt = 0;
    for (size_t i = 0; i < ERROR_GROUP_COUNT; ++i)
        if (expired_groups[i] > 0)
            out[cnt++] = i;
    return cnt;
}
void error_set(ErrorGroup group, ErrorInstance instance, uint32_t timestamp) {
    if (group >= ERROR_GROUP_COUNT || instance >= instances[group])
        return;

    // Push data to the buffer
    ErrorData data = {
        .group = group,
        .instance = instance,
        .timestamp = timestamp,
        .op = _error_set
    };
    if (ring_buffer_push_back(&err_buf, &data) == RING_BUFFER_OK)
        error_routine();
}
void error_reset(ErrorGroup group, ErrorInstance instance) {
    if (instance >= instances[group])
        return;

    // Push data to the buffer
    ErrorData data = {
        .group = group,
        .instance = instance,
        .timestamp = 0,
        .op = _error_reset
    };
    if (ring_buffer_push_back(&err_buf, &data) == RING_BUFFER_OK)
        error_routine();
}
void error_expire(void) {
    // Push data to the buffer
    ErrorData data = { .op = _error_expire };
    if (ring_buffer_push_back(&err_buf, &data) == RING_BUFFER_OK)
        error_routine();
}
void error_expire_immediate(ErrorGroup group, ErrorInstance instance) {
    if (instance >= instances[group])
        return;

    // Push data to the buffer
    ErrorData data = {
        .group = group,
        .instance = instance,
        .op = _error_expire_immediate
    };
    if (ring_buffer_push_back(&err_buf, &data) == RING_BUFFER_OK)
        error_routine();
}
void error_routine(void) {
    // Avoid multiple execution of the routine
    if (routine_lock)
        return;
    routine_lock = true;

    // Check if buffer is not empty
    if (ring_buffer_is_empty(&err_buf)) {
        routine_lock = false;
        return;
    }

    // Execute the right function for the error
    ErrorData err;
    if (ring_buffer_pop_front(&err_buf, &err) == RING_BUFFER_OK)
        err.op(err);

    routine_lock = false;
}

__attribute__((weak)) void error_update_timer_callback(uint32_t timestamp, uint16_t timeout) { }
__attribute__((weak)) void error_stop_timer_callback(void) { }

