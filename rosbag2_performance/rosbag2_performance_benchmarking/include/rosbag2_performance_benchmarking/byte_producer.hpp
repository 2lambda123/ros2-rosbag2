// Copyright 2020, Robotec.ai sp. z o.o.
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

#ifndef ROSBAG2_PERFORMANCE_BENCHMARKING__BYTE_PRODUCER_HPP_
#define ROSBAG2_PERFORMANCE_BENCHMARKING__BYTE_PRODUCER_HPP_

#include <chrono>
#include <memory>
#include <thread>
#include <functional>

#include "rclcpp/utilities.hpp"
#include "msg_utils/helpers.hpp"
#include "rosbag2_performance_benchmarking_msgs/msg/byte_array.hpp"

#include "rosbag2_performance_benchmarking/producer_config.hpp"

class ByteProducer
{
public:
  using producer_initialize_function_t = std::function<void ()>;
  using producer_callback_function_t = std::function<void (
        std::shared_ptr<rosbag2_performance_benchmarking_msgs::msg::ByteArray>)>;

  using producer_finalize_function_t = std::function<void ()>;

  ByteProducer(
    const ProducerConfig & config,
    producer_initialize_function_t producer_initialize,
    producer_callback_function_t producer_callback,
    producer_finalize_function_t producer_finalize)
  : configuration_(config),
    producer_initialize_(producer_initialize),
    producer_callback_(producer_callback),
    producer_finalize_(producer_finalize),
    sleep_time_(configuration_.frequency == 0 ? 1 : 1000 / configuration_.frequency),
    message_(std::make_shared<rosbag2_performance_benchmarking_msgs::msg::ByteArray>())
  {
    msg_utils::helpers::generate_data(*message_, configuration_.message_size);
  }

  void run()
  {
    producer_initialize_();
    for (auto i = 0u; i < configuration_.max_count; ++i) {
      if (!rclcpp::ok()) {
        break;
      }
      producer_callback_(message_);
      std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time_));
    }
    producer_finalize_();
  }

private:
  ProducerConfig configuration_;
  producer_initialize_function_t producer_initialize_;
  producer_callback_function_t producer_callback_;
  producer_finalize_function_t producer_finalize_;
  unsigned int sleep_time_;  // in milliseconds
  // for simplification, this pointer will be reused
  std::shared_ptr<rosbag2_performance_benchmarking_msgs::msg::ByteArray> message_;
};

#endif  // ROSBAG2_PERFORMANCE_BENCHMARKING__BYTE_PRODUCER_HPP_
