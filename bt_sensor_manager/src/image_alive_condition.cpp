#include "bt_sensor_manager/image_alive_condition.hpp"

BT::NodeStatus ImageAliveCondition::onTick(const std::shared_ptr<sensor_msgs::msg::Image>& last_msg)
{
  //last msg는 ROS 토픽에서 수신된 이미지 메시지를 가리키는 포인터
  // 메시지 수신한 타임스탬프를 기록해서 그이후에 얼마동안 메시지가 수신되지 않았는지 체크
  auto now = std::chrono::steady_clock::now();
  if(last_msg)
  {
    last_msg_time_ = now;
    RCLCPP_DEBUG(this->logger(), "[%s] Received Image message", name().c_str());
  }
  auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - last_msg_time_).count();
  if(duration > 5)
  {
    RCLCPP_WARN(this->logger(), "[%s] No Image message for %ld seconds - FAILURE", name().c_str(), duration);
    return BT::NodeStatus::FAILURE;
  }
  return BT::NodeStatus::SUCCESS;
}
