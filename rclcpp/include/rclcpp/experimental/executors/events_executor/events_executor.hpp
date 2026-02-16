// Copyright 2023 iRobot Corporation.
// Copyright 2026 rslcpp contributors – Humble compatibility shim.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef RCLCPP__EXPERIMENTAL__EXECUTORS__EVENTS_EXECUTOR__EVENTS_EXECUTOR_HPP_
#define RCLCPP__EXPERIMENTAL__EXECUTORS__EVENTS_EXECUTOR__EVENTS_EXECUTOR_HPP_

#include <chrono>
#include <limits>
#include <memory>

#include "rclcpp/executors/static_single_threaded_executor.hpp"
#include "rclcpp/experimental/executors/events_executor/events_queue.hpp"
#include "rclcpp/experimental/executors/events_executor/simple_events_queue.hpp"
#include "rclcpp/macros.hpp"
#include "rclcpp/visibility_control.hpp"

#include "rslcpp_time_delay_backend/delay_backend.hpp"

namespace rclcpp
{
namespace experimental
{
namespace executors
{

/// Humble-compatible EventsExecutor shim.
/**
 * On Jazzy / Rolling the EventsExecutor is a fully event-driven executor.
 * On Humble that infrastructure does not exist, so this class wraps the
 * StaticSingleThreadedExecutor while keeping the same public API surface
 * that rslcpp consumers expect:
 *
 *   - Constructor accepting (EventsQueue, bool, ExecutorOptions) or
 *     (ExecutorOptions, EventsQueue, bool).
 *   - spin(), spin_some(), spin_all() – delegated to the base class.
 *   - get_time_until_next_timer() – iterates all known timers.
 */
class EventsExecutor : public rclcpp::executors::StaticSingleThreadedExecutor
{
public:
  RCLCPP_SMART_PTR_DEFINITIONS(EventsExecutor)

  /// Constructor matching the call-site order used in rslcpp.cpp:
  ///   EventsExecutor(events_queue, execute_timers_separate_thread, options)
  RCLCPP_PUBLIC
  explicit EventsExecutor(
    rclcpp::experimental::executors::EventsQueue::UniquePtr /*events_queue*/,
    bool /*execute_timers_separate_thread*/,
    const rclcpp::ExecutorOptions & options = rclcpp::ExecutorOptions())
  : rclcpp::executors::StaticSingleThreadedExecutor(options)
  {}

  /// Constructor matching the Jazzy parameter order (options first).
  RCLCPP_PUBLIC
  explicit EventsExecutor(
    const rclcpp::ExecutorOptions & options = rclcpp::ExecutorOptions(),
    rclcpp::experimental::executors::EventsQueue::UniquePtr /*events_queue*/ =
      std::make_unique<rclcpp::experimental::executors::SimpleEventsQueue>(),
    bool /*execute_timers_separate_thread*/ = false)
  : rclcpp::executors::StaticSingleThreadedExecutor(options)
  {}

  RCLCPP_PUBLIC
  ~EventsExecutor() override = default;

  /// Return the time until the next timer fires across all tracked nodes.
  /**
   * Iterates over every callback group known to the executor, checks every
   * timer, and returns the minimum `time_until_trigger()`.
   * If no timers exist, returns nanoseconds::max().
   */
  RCLCPP_PUBLIC
  std::chrono::nanoseconds get_time_until_next_timer() const
  {
    auto min_time = std::chrono::nanoseconds::max();
    // Gather all callback groups from the executor
    // Use the const-safe accessor that returns weak pointers
    auto groups = const_cast<EventsExecutor *>(this)->get_all_callback_groups();
    for (auto & weak_group : groups) {
      auto group = weak_group.lock();
      if (!group) {
        continue;
      }
      group->find_timer_ptrs_if(
        [&min_time](const rclcpp::TimerBase::SharedPtr & timer) -> bool {
          if (timer) {
            auto t = timer->time_until_trigger();
            if (t < min_time) {
              min_time = t;
            }
          }
          return false;  // continue iterating
        });
    }
    return min_time;
  }

private:
  RCLCPP_DISABLE_COPY(EventsExecutor)
};

}  // namespace executors
}  // namespace experimental
}  // namespace rclcpp

#endif  // RCLCPP__EXPERIMENTAL__EXECUTORS__EVENTS_EXECUTOR__EVENTS_EXECUTOR_HPP_
