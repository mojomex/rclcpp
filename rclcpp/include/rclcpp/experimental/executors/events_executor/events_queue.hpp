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

#ifndef RCLCPP__EXPERIMENTAL__EXECUTORS__EVENTS_EXECUTOR__EVENTS_QUEUE_HPP_
#define RCLCPP__EXPERIMENTAL__EXECUTORS__EVENTS_EXECUTOR__EVENTS_QUEUE_HPP_

#include <queue>

#include "rclcpp/macros.hpp"
#include "rclcpp/visibility_control.hpp"

#include "rclcpp/experimental/executors/events_executor/events_executor_event_types.hpp"

namespace rclcpp
{
namespace experimental
{
namespace executors
{

/// Stub EventsQueue interface for Humble compatibility.
/**
 * On Humble the EventsExecutor is a thin wrapper around
 * StaticSingleThreadedExecutor, so the queue is never really used.
 * This interface exists only so that user code that constructs an
 * EventsExecutor with an explicit queue still compiles.
 */
class EventsQueue
{
public:
  RCLCPP_SMART_PTR_ALIASES_ONLY(EventsQueue)

  RCLCPP_PUBLIC
  EventsQueue() = default;

  RCLCPP_PUBLIC
  virtual ~EventsQueue() = default;

  RCLCPP_PUBLIC
  virtual void
  enqueue(const rclcpp::experimental::executors::ExecutorEvent & event) = 0;

  RCLCPP_PUBLIC
  virtual bool
  dequeue(
    rclcpp::experimental::executors::ExecutorEvent & event,
    std::chrono::nanoseconds timeout = std::chrono::nanoseconds::max()) = 0;

  RCLCPP_PUBLIC
  virtual bool
  empty() const = 0;

  RCLCPP_PUBLIC
  virtual size_t
  size() const = 0;
};

}  // namespace executors
}  // namespace experimental
}  // namespace rclcpp

#endif  // RCLCPP__EXPERIMENTAL__EXECUTORS__EVENTS_EXECUTOR__EVENTS_QUEUE_HPP_
