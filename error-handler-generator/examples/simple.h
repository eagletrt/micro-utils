/*******************************************************************************
 * Critical error handler library generator
 * Generated by error_gen ruby gem, for more information see:
 * https://github.com/eagletrt/micro-utils/tree/master/error-handler-generator
 *
 * Error_gen version 0.1.0-alpha
 * Generation date: 2024-03-19 19:02:48 +0100
 * Generation from: 
 * The error handler contains:
 *     - 3 error groups
 *     - 119 total error instances
 ******************************************************************************/

#ifndef SIMPLE_H
#define SIMPLE_H

#include <stdint.h>
#include <stdbool.h>

// Total number of error instances
#define ERROR_INSTANCE_COUNT 119

// Aliases for the ring buffer critical section functions
// For more info check the docs of the ring buffer library
#define error_cs_enter ring_buffer_cs_enter
#define error_cs_exit ring_buffer_cs_exit

/** @brief Type of the error that categorize a group of instances */
typedef enum {
    ERROR_FIRST,
    ERROR_SECOND,
    ERROR_THIRD,
    ERROR_COUNT
} ErrorGroup;

// Single error instance type definition
typedef uint16_t ErrorInstance;

/**
 * @brief Error type definition
 *
 * @param group The group to which the error belongs
 * @param timestamp The time when the error was set (in ms)
 * @param is_running True if the error is set, false otherwise
 * @param is_expired True if the error has expired, false otherwise
 */
typedef struct {
    ErrorGroup group;
    uint32_t timestamp;
    bool is_running;
    bool is_expired;
} Error;

/** @brief Initialize the internal error handler structures */
void error_init(void);

/**
 * @brief Set an error which will expire after a certain amount of time (the timeout)
 * 
 * @param group The group to which the error belongs
 * @param instance The instance of the error
 * @param The current time (in ms)
 */
void error_set(ErrorGroup group, ErrorInstance instance, uint32_t timestamp);

/**
 * @brief Reset an error to avoid its expiration
 *
 * @param group The group to which the error belongs
 * @param instance The instance of the error
 */
void error_reset(ErrorGroup group, ErrorInstance instance);

/** @brief Set the error as expired */
void error_expire(void);

/**
 * @brief Routine that updates the internal error states
 * @attention This function should not be called inside interrupts callback
 * or other threads
 * @details This function should be called periodically
 */
void error_routine(void);

/**
 * @brief Update the timer that should expire the error after a certain amount of time
 * @attention This function have to be defined by the user
 * @details This function is called internally when an error is set, reset or expired
 *
 * @param timestamp The time in which the error was set (in ms)
 * @param timeout The time that should elapse after the timestamp to expire the error (in ms)
 */
void error_update_timer_callback(uint32_t timestamp, uint16_t timeout);

/**
 * @brief Stop the timer that should expire the errors
 * @attention This function have to be defined by the user
 * @details This function is called internally when an error is reset or expired
 */
void error_stop_timer_callback(void);

#endif  // SIMPLE_H

