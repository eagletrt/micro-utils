# frozen_string_literal: true

module ErrorGen
  module Templates
    HEADER = <<~BANNER
      /*******************************************************************************
       * Critical error handler library generator
       * Generated by error_gen ruby gem, for more information see:
       * https://github.com/eagletrt/micro-utils/tree/master/error-handler-generator
       *
       * Error_gen version <%= ErrorGen::VERSION %>
       * Generation date: <%= Time.now %>
       * Generation from: <%= @jsonfile %>
       * The error handler contains:
       *     - <%= @errors.length %> error groups
       *     - <%= @errors.map(&:instances).inject(0, &:+) %> total error instances
       ******************************************************************************/

    BANNER

    HH = <<~HEADER
      <% safeguard = @target_name.upcase.gsub('-', '_') + '_H' %>
      #ifndef <%= safeguard %>
      #define <%= safeguard %>

      #include <stdint.h>
      #include <stdbool.h>
      <%
        tot_instances = 0
        errors_names = []
        @errors.each { |error|
          name = error.name.upcase.gsub('-', '_')
          errors_names.push(name)
          tot_instances += error.instances
        }
        enum_data = errors_names.join(",\n    ")
      %>
      // Total number of error instances
      <%= '#define ERROR_INSTANCE_COUNT ' + tot_instances.to_s %>

      
      // Aliases for the ring buffer critical section functions
      // For more info check the docs of the ring buffer library
      #define error_cs_enter ring_buffer_cs_enter
      #define error_cs_exit ring_buffer_cs_exit
      
      /** @brief Type of the error that categorize a group of instances */
      typedef enum {
          <%= enum_data %>,
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
      
      #endif  // <%= safeguard %>

    HEADER

    # TODO: Add general error that expire immediately if something goes wrong in this library
    # TODO: Better find function (apply heuristics to reduce complexity)
    # TODO: Remove element without index in O(log N)
    CC = <<~SOURCE
      #include "<%= @target_name %>.h"

      #include <stddef.h>

      #include "ring-buffer.h"
      #include "min-heap.h"
      <%
        instances_arr = []
        timeouts_arr = []
        groups_arr = [] 
        errors_arrays = []
        errors_instances = ''
        errors_init = []
        @errors.each { |error|
          name = error.name.upcase.gsub('-', '_')
          instances_arr.push('[' + name + '] = ' + error.instances.to_s)
          timeouts_arr.push('[' + name + '] = ' + error.timeout.to_s)

          inst_name = name.downcase + '_instances'
          errors_arrays.push(inst_name)
          errors_instances += 'static Error ' + inst_name + '[' + error.instances.to_s + "];\n"
          groups_arr.push('[' + name + '] = ' + inst_name)
          errors_init.push('for (size_t i = 0; i < instances[' + name + ']; ++i)\n        ' + inst_name + '[i].group = ' + name + ';')
        }
        init = errors_init.join("\n    ")
        instances = instances_arr.join(",\n    ")
        timeouts = timeouts_arr.join(",\n    ")
        groups = groups_arr.join(",\n    ")
      %>
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

      // Total number of instances for each group
      static const uint16_t instances[] = {
          <%= instances %>
      };
      // Error timeout for each group
      static const uint16_t timeouts[] = {
          <%= timeouts %>
      };

      // Errors information
      <%= errors_instances %>
      static Error * errors[] = {
          <%= groups %>
      };

      // Function declaration needed for the min heap
      int8_t _error_compare(void * a, void * b);

      bool routine_lock = false;
      RingBuffer(ErrorData, ERROR_BUFFER_SIZE) err_buf = ring_buffer_init(ErrorData, ERROR_BUFFER_SIZE);
      MinHeap(Error *, ERROR_INSTANCE_COUNT) running_errors = min_heap_init(Error *, ERROR_INSTANCE_COUNT, _error_compare);

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

          // Add error to the running list of errors and
          // update timer if the error is the first to expire
          if (!min_heap_insert(&running_errors, &err))
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

          // Get the current first element
          Error * top = NULL;
          if (!min_heap_top(&running_errors, &top))
              return;

          if (top == err) {
              // If the removed error is the first in the heap
              // remove it and update (or stop) the timer
              if (!min_heap_remove(&running_errors, 0, NULL)) return;
              if (min_heap_is_empty(&running_errors))
                  error_stop_timer_callback();
              else if (min_heap_top(&running_errors, &top))
                  error_update_timer_callback(top->timestamp, timeouts[top->group]);
          }
          else {
              // Find and remove the error
              ssize_t i = min_heap_find(&running_errors, &err);
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
          if (!min_heap_top(&running_errors, &top))
              return;

          if (!top->is_running || top->is_expired)
              return;

          Error * prev = top;
          do {
              // Update error info
              top->is_running = false;
              top->is_expired = true;

              // Get next error and remove the previous
              if (!min_heap_remove(&running_errors, 0, NULL))
                  break;

              // Stop the timer if there are no more errors
              if (min_heap_is_empty(&running_errors)) {
                  error_stop_timer_callback();
                  return;
              }

              // Get next errors
              if (!min_heap_top(&running_errors, &top))
                  break;
          } while(_error_compare(&top, &prev) <= 0);
          
          // Update the timer
          if (top != NULL)
              error_update_timer_callback(top->timestamp, timeouts[top->group]);
      }

      void error_init(void) {
          <%= init %>
      }
      void error_set(ErrorGroup group, ErrorInstance instance, uint32_t timestamp) {
          if (group >= ERROR_COUNT || instance >= instances[group])
              return;

          // Push data to the buffer
          ErrorData data = {
              .group = group,
              .instance = instance,
              .timestamp = timestamp,
              .op = _error_set
          };
          if (ring_buffer_push_back(&err_buf, &data))
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
          if (ring_buffer_push_back(&err_buf, &data))
              error_routine();
      }
      void error_expire(void) {
          // Push data to the buffer
          ErrorData data = { .op = _error_expire };
          if (ring_buffer_push_back(&err_buf, &data))
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
          if (ring_buffer_pop_front(&err_buf, &err))
              err.op(err);

          routine_lock = false;
      }

      __attribute__((weak)) void error_update_timer_callback(uint32_t timestamp, uint16_t timeout) { }
      __attribute__((weak)) void error_stop_timer_callback(void) { }

    SOURCE
  end
end
