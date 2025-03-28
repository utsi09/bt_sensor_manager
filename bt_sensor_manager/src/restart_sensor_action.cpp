#include "bt_sensor_manager/restart_sensor_action.hpp"
#include <chrono>
#include <thread>
#include <rclcpp/executors/single_threaded_executor.hpp>
#include <rclcpp/executors/multi_threaded_executor.hpp>
// 두 가지 타입 다 include
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>

BT::NodeStatus RestartSensorAction::tick()
{
  // 1) "cmd" 입력 포트로부터 명령어 받기
  auto cmd_res = getInput<std::string>("cmd");
  if (!cmd_res)
  {
    RCLCPP_ERROR(rclcpp::get_logger("RestartSensorAction"),
                 "Failed to get input [cmd]: %s", cmd_res.error().c_str());
    return BT::NodeStatus::FAILURE;
  }
  std::string command = cmd_res.value();
  RCLCPP_INFO(rclcpp::get_logger("RestartSensorAction"),
              "[%s] Executing command: %s", name().c_str(), command.c_str());

  // 명령어 실행
  int ret = std::system(command.c_str());
  (void)ret; // 여기서는 반환값을 사용하지 않음

  // 2) "topic_name" 입력 포트로부터 토픽 이름 받기
  auto topic_res = getInput<std::string>("topic_name");
  if (!topic_res)
  {
    RCLCPP_ERROR(rclcpp::get_logger("RestartSensorAction"),
                 "Failed to get input [topic_name]: %s", topic_res.error().c_str());
    return BT::NodeStatus::FAILURE;
  }
  std::string topic_name = topic_res.value();

  // 3) "sensor_type" 입력 포트로부터 센서 타입 받기
  auto sensor_type_res = getInput<std::string>("sensor_type");
  if (!sensor_type_res)
  {
    RCLCPP_ERROR(rclcpp::get_logger("RestartSensorAction"),
                 "Failed to get input [sensor_type]: %s", sensor_type_res.error().c_str());
    return BT::NodeStatus::FAILURE;
  }
  std::string sensor_type = sensor_type_res.value();

  // 메시지 수신 확인 변수
  bool received = false;

  // 임시 노드 생성

  rclcpp::executors::MultiThreadedExecutor executor;
  auto node = std::make_shared<rclcpp::Node>("bt_sensor_manager");
  executor.add_node(node);

  // 4) sensor_type에 따라 구독 로직 분기
  if (sensor_type == "image")
  {
    auto callback = [&received](const sensor_msgs::msg::Image::SharedPtr) {
      received = true;
    };
    auto sub = node->create_subscription<sensor_msgs::msg::Image>(
        topic_name, 10, callback);
    RCLCPP_INFO(rclcpp::get_logger("RestartSensorAction"),
                "[%s] Subscribing to Image topic: %s", name().c_str(), topic_name.c_str());
  }
  else if (sensor_type == "pointcloud2")
  {
    auto callback = [&received](const sensor_msgs::msg::PointCloud2::SharedPtr) {
      received = true;
    };
    auto sub = node->create_subscription<sensor_msgs::msg::PointCloud2>(
        topic_name, 10, callback);
    RCLCPP_INFO(rclcpp::get_logger("RestartSensorAction"),
                "[%s] Subscribing to PointCloud2 topic: %s", name().c_str(), topic_name.c_str());
  }
  else
  {
    RCLCPP_ERROR(rclcpp::get_logger("RestartSensorAction"),
                 "Unknown sensor_type: %s", sensor_type.c_str());
    return BT::NodeStatus::FAILURE;
  }

  // 5) 10초 동안 토픽 메시지 수신 대기
  auto start_time = std::chrono::steady_clock::now();
  while (!received)
  {
    executor.spin_once(std::chrono::milliseconds(100));
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count();
    if(elapsed >= 10)
    {
      RCLCPP_WARN(rclcpp::get_logger("RestartSensorAction"),
                  "[%s] No message on topic '%s' for 10 seconds - FAILURE",
                  name().c_str(), topic_name.c_str());
      return BT::NodeStatus::FAILURE;
    }
  }

  // 메시지 수신 확인 시 SUCCESS 반환
  return BT::NodeStatus::SUCCESS;
}
