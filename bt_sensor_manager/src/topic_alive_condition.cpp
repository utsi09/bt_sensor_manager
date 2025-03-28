#include "bt_sensor_manager/topic_alive_condition.hpp"

//포인트 클라우드 토픽을 가르키는 포인터를 받아서 타임스탬프판단
BT::NodeStatus TopicAliveCondition::onTick(const std::shared_ptr<sensor_msgs::msg::PointCloud2>& last_msg)
{
  auto now = std::chrono::steady_clock::now();
  if(last_msg)
  {
    last_msg_time_ = now;
    RCLCPP_DEBUG(this->logger(), "[%s] Received PointCloud2 message", name().c_str());
  }
  auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - last_msg_time_).count();
  if(duration > 3)
  {
    RCLCPP_WARN(this->logger(), "[%s] No PointCloud2 message for %ld seconds - FAILURE", name().c_str(), duration);
    return BT::NodeStatus::FAILURE;
  }
  return BT::NodeStatus::SUCCESS;
}
