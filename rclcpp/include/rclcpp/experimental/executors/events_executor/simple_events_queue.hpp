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

#ifndef RCLCPP__EXPERIMENTAL__EXECUTORS__EVENTS_EXECUTOR__SIMPLE_EVENTS_QUEUE_HPP_
#define RCLCPP__EXPERIMENTAL__EXECUTORS__EVENTS_EXECUTOR__SIMPLE_EVENTS_QUEUE_HPP_

#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>

#include "rclcpp/experimental/executors/events_executor/events_queue.hpp"

namespace rclcpp
{
namespace experimental
{
namespace executors
{

/// Stub SimpleEventsQueue for Humble compatibility.
/**
 * On Humble the EventsExecutor is a thin wrapper around
 * StaticSingleThreadedExecutor, so this queue is never actually consumed.
 * It exists only so that user code that passes a SimpleEventsQueue to the
 * EventsExecutor constructor still compiles.
 */
class SimpleEventsQueue : public EventsQueue
{
public:
  RCLCPP_PUBLIC
  ~SimpleEventsQueue() override = default;

  RCLCPP_PUBLIC
  void
  enqueue(const rclcpp::experimental::executors::ExecutorEvent & /*event*/) override {}

  RCLCPP_PUBLIC
  bool
  dequeue(
    rclcpp::experimental::executors::ExecutorEvent & /*event*/,
    std::chrono::nanoseconds /*timeout*/ = std::chrono::nanoseconds::max()) override
  {
    return false;
  }

  RCLCPP_PUBLIC
  bool
  empty() const override
  {
    return true;
  }

  RCLCPP_PUBLIC
  size_t
  size() const override
  {
    return 0;
  }
};

}  // namespace executors
}  // namespace experimental
}  // namespace rclcpp

#endif  // RCLCPP__EXPERIMENTAL__EXECUTORS__EVENTS_EXECUTOR__SIMPLE_EVENTS_QUEUE_HPP_
