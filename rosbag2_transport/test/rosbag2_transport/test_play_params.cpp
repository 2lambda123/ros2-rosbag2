// Copyright 2023, Patrick Roncagliolo.
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

#include <gmock/gmock.h>

#include <chrono>
#include <future>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <utility>

#include "rclcpp/rclcpp.hpp"
#include "mock_player.hpp"
#include "rosbag2_play_test_fixture.hpp"
#include "rosbag2_storage/storage_options.hpp"
#include "rosbag2_transport/play_options.hpp"

TEST_F(RosBag2PlayTestFixture, parse_parameter_from_file) {
  // _SRC_RESOURCES_DIR_PATH defined in CMakeLists.txt
  rclcpp::NodeOptions opts;
  opts.arguments(
  {
    "--ros-args",
    "--params-file", _SRC_RESOURCES_DIR_PATH "/player_node_params.yaml"
  });
  opts.append_parameter_override(
    "qos_profile_overrides_path",
    _SRC_RESOURCES_DIR_PATH "/qos_profile_overrides.yaml");
  opts.append_parameter_override(
    "uri",
    _SRC_RESOURCES_DIR_PATH "/sqlite3/test_bag_for_seek");

  auto node = std::make_shared<MockPlayer>("player_params_node", opts);
  auto play_options = node->get_play_options();
  auto storage_options = node->get_storage_options();
  YAML::Node yaml_play_opt = YAML::convert<rosbag2_transport::PlayOptions>().encode(play_options);
  YAML::Node yaml_storage_opt = YAML::convert<rosbag2_storage::StorageOptions>().encode(
    storage_options);

  auto param_node = YAML::LoadFile(_SRC_RESOURCES_DIR_PATH "/player_node_params.yaml");
  auto qos_node = YAML::LoadFile(_SRC_RESOURCES_DIR_PATH "/qos_profile_overrides.yaml");

  YAML::Emitter emitter;
  emitter
    << YAML::Newline << YAML::Comment("player_node_params.yaml")
    << param_node << YAML::Newline
    << YAML::Newline << YAML::Comment("qos_profile_overrides.yaml")
    << qos_node << YAML::Newline
    << YAML::Newline << YAML::Comment("node play parameters")
    << yaml_play_opt << YAML::Newline
    << YAML::Newline << YAML::Comment("node storage parameters")
    << yaml_storage_opt << YAML::Newline;

  std::cout << emitter.c_str() << std::endl;

  // TODO(roncapat): compare YAML trees (from file vs from struct)
}

/*
TEST_F(RosBag2PlayTestFixture, test_negative_durations) {
// TODO(roncapat): implement
}
*/
