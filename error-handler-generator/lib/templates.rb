# frozen_string_literal: true

module ErrorGen
  module Templates
    # TODO: Refactor
    HEADER = <<~BANNER
      /*******************************************************************************
       * Critical error handler library generator
       * Generated by error_gen ruby gem, for more information see:
       * https://github.com/eagletrt/micro-utils/tree/master/error-handler-generator
       *
       * Error_gen version <%= ErrorGen::VERSION %>
       * Generation date: <%= Time.now %>
       * Generated from: <%= @jsonfile %>
       * With prefix: <%= unless @prefix.nil? then @prefix else 'none' end %>
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
      #include <stddef.h>
      <%
        tot_instances = 0
        errors_names = []
        @errors.each { |error|
          name = (@prefix&.+('_')&.upcase.to_s) + 'ERROR_GROUP_' + error.name.upcase.gsub('-', '_')
          errors_names.push(name)
          tot_instances += error.instances
        }
        enum_data = errors_names.join(",\n    ")
      %>
      // Total number of error instances
      <%= '#define ' + @prefix&.+('_')&.upcase.to_s + 'ERROR_INSTANCE_COUNT ' + tot_instances.to_s %>


      /**
       * @brief Set or reset an instance of an error based on a condition
       *
       * @details If the condition is true the error is set, otherwise it is reset
       *
       * @param condition A boolean expression
       * @param group The group to which the error belongs
       * @param instance The instance of the error
       * @param The current time (in ms)
       */
      #define <%= @prefix&.+('_')&.upcase.to_s %>ERROR_TOGGLE_IF(condition, group, instance, timestamp) \\
          ((condition) ? <%= @prefix&.+('_').to_s %>error_set(group, instance, timestamp) : <%= @prefix&.+('_').to_s %>error_reset(group, instance))

      /** @brief Type of the error that categorize a group of instances */
      typedef enum {
          <%= enum_data %>,
          <%= @prefix&.+('_')&.upcase.to_s %>ERROR_GROUP_COUNT
      } <%= @prefix&.camelize.to_s %>ErrorGroup;

      // Single error instance type definition
      typedef uint16_t <%= @prefix&.camelize.to_s %>ErrorInstance;

      /**
       * @brief Error type definition
       *
       * @param group The group to which the error belongs
       * @param timestamp The time when the error was set (in ms)
       * @param is_running True if the error is set, false otherwise
       * @param is_expired True if the error has expired, false otherwise
       */
      typedef struct {
          <%= @prefix&.camelize.to_s %>ErrorGroup group;
          uint32_t timestamp;
          bool is_running;
          bool is_expired;
      } <%= @prefix&.camelize.to_s %>Error;

      /**
       * @brief Initialize the internal error handler structures
       *
       * @details A critical section is defined as a block of code where, if an interrupt
       * happens, undefined behaviour with the modified data within the block can happen
       *
       * @param cs_enter A pointer to a function that should manage a critical section
       * @param cs_exit A pointer to a function that shuold manage a critical section
       */
      void <%= @prefix&.+('_').to_s %>error_init(void (* cs_enter)(void), void (* cs_exit)(void));

      /**
       * @brief Get the number of errors that has been set but they still have to expire
       *
       * @param size_t The number of running errors
       */
      size_t <%= @prefix&.+('_').to_s %>error_get_running(void);

      /**
       * @brief Get the number of expired errors
       *
       * @param size_t The number of expired errors
       */
      size_t <%= @prefix&.+('_').to_s %>error_get_expired(void);

      /**
       * @brief Get the number of running error of a specific group
       *
       * @param group The error group
       *
       * @return uint16_t The number of running errors
       */
      uint16_t <%= @prefix&.+('_').to_s %>error_get_group_running(<%= @prefix&.camelize.to_s %>ErrorGroup group);

      /**
       * @brief Get the number of expired error of a specific group
       *
       * @param group The error group
       *
       * @return uint16_t The number of running errors
       */
      uint16_t <%= @prefix&.+('_').to_s %>error_get_group_expired(<%= @prefix&.camelize.to_s %>ErrorGroup group);

      /**
       * @brief Get a copy of all the errors that are currently running
       *
       * @attention This function can be quite expensive in terms of time
       * and should be used wisely, do not call to often
       * @attention This function calls the critical section handler functions
       *
       * @details The out array should be able to contain all the instances
       *
       * @param out A pointer to an array of errors where the data is copied into
       *
       * @return size_t The number of copied errors
       */
      size_t <%= @prefix&.+('_').to_s %>error_dump_running(<%= @prefix&.camelize.to_s %>Error * out);

      /**
       * @brief Get a copy of all the errors that are expired
       *
       * @attention This function can be quite expensive in terms of time
       * and should be used wisely, do not call to often
       * @attention This function calls the critical section handler functions
       *
       * @details The out array should be able to contain all the instances
       *
       * @param out A pointer to an array of errors where the data is copied into
       *
       * @return size_t The number of copied errors
       */
      size_t <%= @prefix&.+('_').to_s %>error_dump_expired(<%= @prefix&.camelize.to_s %>Error * out);

      /**
       * @brief Get all the groups in which at least one error is running
       *
       * @param out A pointer to an array of groups where the data is copied into
       *
       * @return size_t The number of copied groups
       */
      size_t <%= @prefix&.+('_').to_s %>error_dump_running_groups(<%= @prefix&.camelize.to_s %>ErrorGroup * out);

      /**
       * @brief Get all the groups in which at least one error is expired
       *
       * @param out A pointer to an array of groups where the data is copied into
       *
       * @return size_t The number of copied groups
       */
      size_t <%= @prefix&.+('_').to_s %>error_dump_expired_groups(<%= @prefix&.camelize.to_s %>ErrorGroup * out);

      /**
       * @brief Set an error which will expire after a certain amount of time (the timeout)
       *
       * @param group The group to which the error belongs
       * @param instance The instance of the error
       * @param The current time (in ms)
       */
      void <%= @prefix&.+('_').to_s %>error_set(<%= @prefix&.camelize.to_s %>ErrorGroup group, <%= @prefix&.camelize.to_s %>ErrorInstance instance, uint32_t timestamp);

      /**
       * @brief Reset an error to avoid its expiration
       *
       * @param group The group to which the error belongs
       * @param instance The instance of the error
       */
      void <%= @prefix&.+('_').to_s %>error_reset(<%= @prefix&.camelize.to_s %>ErrorGroup group, <%= @prefix&.camelize.to_s %>ErrorInstance instance);

      /** @brief Set the error as expired */
      void <%= @prefix&.+('_').to_s %>error_expire(void);

      /**
       * @brief Set the error as expired immediately even if it is not running
       *
       * @param group The group to which the error belongs
       * @param instance The instance of the error
       */
      void <%= @prefix&.+('_').to_s %>error_expire_immediate(<%= @prefix&.camelize.to_s %>ErrorGroup group, <%= @prefix&.camelize.to_s %>ErrorInstance instance);

      /**
       * @brief Routine that updates the internal error states
       *
       * @attention This function should not be called inside interrupts callback
       * or other threads
       *
       * @details This function should be called periodically
       */
      void <%= @prefix&.+('_').to_s %>error_routine(void);

      /**
       * @brief Update the timer that should expire the error after a certain amount of time
       *
       * @attention This function have to be defined by the user
       *
       * @details This function is called internally when an error is set, reset or expired
       *
       * @param timestamp The time in which the error was set (in ms)
       * @param timeout The time that should elapse after the timestamp to expire the error (in ms)
       */
      void <%= @prefix&.+('_').to_s %>error_update_timer_callback(uint32_t timestamp, uint16_t timeout);

      /**
       * @brief Stop the timer that should expire the errors
       *
       * @attention This function have to be defined by the user
       *
       * @details This function is called internally when an error is reset or expired
       */
      void <%= @prefix&.+('_').to_s %>error_stop_timer_callback(void);

      #endif  // <%= safeguard %>

    HEADER

    # TODO: Add general error that expire immediately if something goes wrong in this library
    # TODO: Add callback that is called after an error is expired an give useful info
    #       about it as a parameter
    # TODO: Better find function (apply heuristics to reduce complexity)
    # TODO: Remove element without index in O(log N)
    CC = <<~SOURCE
      #include "<%= @target_name %>.h"

      #include <string.h>

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
          instances_arr.push('[' + @prefix&.+('_')&.upcase.to_s + 'ERROR_GROUP_' + name + '] = ' + error.instances.to_s)
          timeouts_arr.push('[' + @prefix&.+('_')&.upcase.to_s + 'ERROR_GROUP_' + name + '] = ' + error.timeout.to_s)

          inst_name = @prefix&.+('_').to_s + 'error_' + name.downcase + '_instances'
          errors_arrays.push(inst_name)
          errors_instances += 'static ' + @prefix&.camelize.to_s + 'Error ' + inst_name + '[' + error.instances.to_s + "];\n"
          groups_arr.push('[' + @prefix&.+('_')&.upcase.to_s + 'ERROR_GROUP_' + name + '] = ' + inst_name)
          errors_init.push('for (size_t i = 0; i < instances[' + @prefix&.+('_')&.upcase.to_s + 'ERROR_GROUP_' + name + ']; ++i)\n        ' + inst_name + '[i].group = ' + @prefix&.+('_')&.upcase.to_s + 'ERROR_GROUP_' + name + ';')
        }
        init = errors_init.join("\n    ")
        instances = instances_arr.join(",\n    ")
        timeouts = timeouts_arr.join(",\n    ")
        groups = groups_arr.join(",\n    ")
      %>
      // Ring buffer maximum number of elements
      #define <%= @prefix&.+('_')&.upcase.to_s %>ERROR_BUFFER_SIZE 16

      /**
       * @brief Error data type definition needed to manage the errors
       *
       * @param group The group to which the error belongs
       * @param instance The instance of the error
       * @param timestamp The current time (in ms)
       * @param op A pointer to the operation that needs to be executed
       */
      typedef struct _<%= @prefix&.camelize.to_s %>ErrorData {
          <%= @prefix&.camelize.to_s %>ErrorGroup group;
          <%= @prefix&.camelize.to_s %>ErrorInstance instance;
          uint32_t timestamp;
          void (* op)(struct _<%= @prefix&.camelize.to_s %>ErrorData);
      } <%= @prefix&.camelize.to_s %>ErrorData;

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
      static <%= @prefix&.camelize.to_s %>Error * errors[] = {
          <%= groups %>
      };

      // Function declaration needed for the min heap
      int8_t _<%= @prefix&.+('_').to_s %>error_compare(void * a, void * b);

      bool routine_lock = false;
      RingBuffer(<%= @prefix&.camelize.to_s %>ErrorData, <%= @prefix&.+('_')&.upcase.to_s %>ERROR_BUFFER_SIZE) err_buf;
      RingBuffer(<%= @prefix&.camelize.to_s %>Error *, <%= @prefix&.+('_')&.upcase.to_s %>ERROR_INSTANCE_COUNT) expired_errors = ring_buffer_new(<%= @prefix&.camelize.to_s %>Error *, <%= @prefix&.+('_')&.upcase.to_s %>ERROR_INSTANCE_COUNT, NULL, NULL);
      MinHeap(<%= @prefix&.camelize.to_s %>Error *, <%= @prefix&.+('_')&.upcase.to_s %>ERROR_INSTANCE_COUNT) running_errors = min_heap_new(<%= @prefix&.camelize.to_s %>Error *, <%= @prefix&.+('_')&.upcase.to_s %>ERROR_INSTANCE_COUNT, _<%= @prefix&.+('_').to_s %>error_compare);

      // Fast lookup for groups that are running or expired
      uint16_t running_groups[<%= @prefix&.+('_')&.upcase.to_s %>ERROR_GROUP_COUNT];
      uint16_t expired_groups[<%= @prefix&.+('_')&.upcase.to_s %>ERROR_GROUP_COUNT];

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
      int8_t _<%= @prefix&.+('_').to_s %>error_compare(void * a, void * b) {
          <%= @prefix&.camelize.to_s %>Error * e1 = *(<%= @prefix&.camelize.to_s %>Error **)a;
          <%= @prefix&.camelize.to_s %>Error * e2 = *(<%= @prefix&.camelize.to_s %>Error **)b;
          int32_t t1 = e1->timestamp + timeouts[e1->group];
          int32_t t2 = e2->timestamp + timeouts[e2->group];
          return t1 < t2 ? -1 : (t1 == t2 ? 0 : 1);
      }

      /**
       * @brief Set the error if possible and update the timer if necessary
       *
       * @param data The data of the error to set
       */
      void _<%= @prefix&.+('_').to_s %>error_set(<%= @prefix&.camelize.to_s %>ErrorData data) {
          // Get error
          <%= @prefix&.camelize.to_s %>Error * err = &errors[data.group][data.instance];
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
          <%= @prefix&.camelize.to_s %>Error ** top = (<%= @prefix&.camelize.to_s %>Error **)min_heap_peek(&running_errors);
          if (top != NULL && *top == err)
              <%= @prefix&.+('_').to_s %>error_update_timer_callback(err->timestamp, timeouts[err->group]);
      }

      /**
       * @brief Reset the error if possible and update the timer if necessary
       *
       * @param data The data of the error to reset
       */
      void _<%= @prefix&.+('_').to_s %>error_reset(<%= @prefix&.camelize.to_s %>ErrorData data) {
          // Get error
          <%= @prefix&.camelize.to_s %>Error * err = &errors[data.group][data.instance];
          if (!err->is_running || err->is_expired)
              return;

          // Update error info
          err->is_running = false;
          --running_groups[data.group];

          // Get the current first element
          <%= @prefix&.camelize.to_s %>Error * top = NULL;
          if (min_heap_top(&running_errors, &top) != MIN_HEAP_OK)
              return;

          if (top == err) {
              // If the removed error is the first in the heap
              // remove it and update (or stop) the timer
              if (min_heap_remove(&running_errors, 0, NULL) != MIN_HEAP_OK)
                  return;

              if (min_heap_is_empty(&running_errors))
                  <%= @prefix&.+('_').to_s %>error_stop_timer_callback();
              else if (min_heap_top(&running_errors, &top) == MIN_HEAP_OK)
                  <%= @prefix&.+('_').to_s %>error_update_timer_callback(top->timestamp, timeouts[top->group]);
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
      void _<%= @prefix&.+('_').to_s %>error_expire(<%= @prefix&.camelize.to_s %>ErrorData data) {
          // Get error
          <%= @prefix&.camelize.to_s %>Error * top = NULL;
          if (min_heap_top(&running_errors, &top) != MIN_HEAP_OK)
              return;

          if (!top->is_running || top->is_expired)
              return;

          <%= @prefix&.camelize.to_s %>Error * prev = top;
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
                  <%= @prefix&.+('_').to_s %>error_stop_timer_callback();
                  return;
              }

              // Get next errors
              if (min_heap_top(&running_errors, &top) != MIN_HEAP_OK)
                  break;
          } while(_<%= @prefix&.+('_').to_s %>error_compare(&top, &prev) <= 0);

          // Update the timer
          if (top != NULL)
              <%= @prefix&.+('_').to_s %>error_update_timer_callback(top->timestamp, timeouts[top->group]);
      }
      /**
       * @brief Expire the error immediately without waiting for the timer
       *
       * @param data The data of the error to expire
       */
      void _<%= @prefix&.+('_').to_s %>error_expire_immediate(<%= @prefix&.camelize.to_s %>ErrorData data) {
          // Get error
          <%= @prefix&.camelize.to_s %>Error * err = &errors[data.group][data.instance];
          if (err->is_expired)
              return;

          // Update error info
          err->is_running = false;
          err->is_expired = true;
          --running_groups[data.group];
          ++expired_groups[data.group];

          // Add error to the list of expired errors
          if (ring_buffer_push_back(&expired_errors, &err) != RING_BUFFER_OK)
              break;

          // Remove the error from the list of running errors
          signed_size_t index = min_heap_find(&running_errors, &err);
          if (index < 0)
              return;
          if (min_heap_remove(&running_errors, index, NULL) != MIN_HEAP_OK)
              return;

          // Stop the timer if there are no more errors
          if (min_heap_is_empty(&running_errors)) {
              <%= @prefix&.+('_').to_s %>error_stop_timer_callback();
              return;
          }
          else if (index == 0) {
              // Update the timer with the next error data
              <%= @prefix&.camelize.to_s %>Error ** next = (<%= @prefix&.camelize.to_s %>Error **)min_heap_peek(&running_errors);
              if (next != NULL)
                  <%= @prefix&.+('_').to_s %>error_update_timer_callback((*next)->timestamp, timeouts[(*next)->group]);
          }
      }

      void <%= @prefix&.+('_').to_s %>error_init(void (* cs_enter)(void), void (* cs_exit)(void)) {
          ring_buffer_init(&err_buf, <%= @prefix&.camelize.to_s %>ErrorData, <%= @prefix&.+('_')&.upcase.to_s %>ERROR_BUFFER_SIZE, cs_enter, cs_exit);

          <%= init %>
      }
      size_t <%= @prefix&.+('_').to_s %>error_get_running(void) {
          return min_heap_size(&running_errors);
      }
      size_t <%= @prefix&.+('_').to_s %>error_get_expired(void) {
          return ring_buffer_size(&expired_errors);
      }
      uint16_t <%= @prefix&.+('_').to_s %>error_get_group_running(<%= @prefix&.camelize.to_s %>ErrorGroup group) {
          if (group >= <%= @prefix&.+('_')&.upcase.to_s %>ERROR_GROUP_COUNT)
              return 0U;
          return running_groups[group];
      }
      uint16_t <%= @prefix&.+('_').to_s %>error_get_group_expired(<%= @prefix&.camelize.to_s %>ErrorGroup group) {
          if (group >= <%= @prefix&.+('_')&.upcase.to_s %>ERROR_GROUP_COUNT)
              return 0U;
          return expired_groups[group];
      }
      size_t <%= @prefix&.+('_').to_s %>error_dump_running(<%= @prefix&.camelize.to_s %>Error * out) {
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
      size_t <%= @prefix&.+('_').to_s %>error_dump_expired(<%= @prefix&.camelize.to_s %>Error * out) {
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
      size_t <%= @prefix&.+('_').to_s %>error_dump_running_groups(<%= @prefix&.camelize.to_s %>ErrorGroup * out) {
          if (out == NULL)
              return 0U;
          // Copy data
          size_t cnt = 0;
          for (size_t i = 0; i < <%= @prefix&.+('_')&.upcase.to_s %>ERROR_GROUP_COUNT; ++i)
              if (running_groups[i] > 0)
                  out[cnt++] = i;
          return cnt;
      }
      size_t <%= @prefix&.+('_').to_s %>error_dump_expired_groups(<%= @prefix&.camelize.to_s %>ErrorGroup * out) {
          if (out == NULL)
              return 0U;
          // Copy data
          size_t cnt = 0;
          for (size_t i = 0; i < <%= @prefix&.+('_')&.upcase.to_s %>ERROR_GROUP_COUNT; ++i)
              if (expired_groups[i] > 0)
                  out[cnt++] = i;
          return cnt;
      }
      void <%= @prefix&.+('_').to_s %>error_set(<%= @prefix&.camelize.to_s %>ErrorGroup group, <%= @prefix&.camelize.to_s %>ErrorInstance instance, uint32_t timestamp) {
          if (group >= <%= @prefix&.+('_')&.upcase.to_s %>ERROR_GROUP_COUNT || instance >= instances[group])
              return;

          // Push data to the buffer
          <%= @prefix&.camelize.to_s %>ErrorData data = {
              .group = group,
              .instance = instance,
              .timestamp = timestamp,
              .op = _<%= @prefix&.+('_').to_s %>error_set
          };
          if (ring_buffer_push_back(&err_buf, &data) == RING_BUFFER_OK)
              <%= @prefix&.+('_').to_s %>error_routine();
      }
      void <%= @prefix&.+('_').to_s %>error_reset(<%= @prefix&.camelize.to_s %>ErrorGroup group, <%= @prefix&.camelize.to_s %>ErrorInstance instance) {
          if (instance >= instances[group])
              return;

          // Push data to the buffer
          <%= @prefix&.camelize.to_s %>ErrorData data = {
              .group = group,
              .instance = instance,
              .timestamp = 0,
              .op = _<%= @prefix&.+('_').to_s %>error_reset
          };
          if (ring_buffer_push_back(&err_buf, &data) == RING_BUFFER_OK)
              <%= @prefix&.+('_').to_s %>error_routine();
      }
      void <%= @prefix&.+('_').to_s %>error_expire(void) {
          // Push data to the buffer
          <%= @prefix&.camelize.to_s %>ErrorData data = { .op = _<%= @prefix&.+('_').to_s %>error_expire };
          if (ring_buffer_push_back(&err_buf, &data) == RING_BUFFER_OK)
              <%= @prefix&.+('_').to_s %>error_routine();
      }
      void <%= @prefix&.+('_').to_s %>error_expire_immediate(<%= @prefix&.camelize.to_s %>ErrorGroup group, <%= @prefix&.camelize.to_s %>ErrorInstance instance) {
          if (instance >= instances[group])
              return;

          // Push data to the buffer
          <%= @prefix&.camelize.to_s %>ErrorData data = {
              .group = group,
              .instance = instance,
              .op = _<%= @prefix&.+('_').to_s %>error_expire_immediate
          };
          if (ring_buffer_push_back(&err_buf, &data) == RING_BUFFER_OK)
              <%= @prefix&.+('_').to_s %>error_routine();
      }
      void <%= @prefix&.+('_').to_s %>error_routine(void) {
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
          <%= @prefix&.camelize.to_s %>ErrorData err;
          if (ring_buffer_pop_front(&err_buf, &err) == RING_BUFFER_OK)
              err.op(err);

          routine_lock = false;
      }

      __attribute__((weak)) void <%= @prefix&.+('_').to_s %>error_update_timer_callback(uint32_t timestamp, uint16_t timeout) { }
      __attribute__((weak)) void <%= @prefix&.+('_').to_s %>error_stop_timer_callback(void) { }

    SOURCE
  end
end
