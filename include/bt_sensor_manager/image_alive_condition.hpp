#ifndef IMAGE_ALIVE_CONDITION_HPP
#define IMAGE_ALIVE_CONDITION_HPP

#include <behaviortree_ros2/bt_topic_sub_node.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <rclcpp/rclcpp.hpp>
#include <chrono>

using namespace std::chrono_literals;

// 이미지 타입의 토픽을 가르키는 포인터를 받아서
class ImageAliveCondition : public BT::RosTopicSubNode<sensor_msgs::msg::Image>
{
public:
  ImageAliveCondition(const std::string & name, const BT::NodeConfig & config, const BT::RosNodeParams & params)
    : BT::RosTopicSubNode<sensor_msgs::msg::Image>(name, config, params)
  {
    last_msg_time_ = std::chrono::steady_clock::now();
  }

  static BT::PortsList providedPorts()
  {
    return { BT::InputPort<std::string>("topic_name") };
  }

protected:
  BT::NodeStatus onTick(const std::shared_ptr<sensor_msgs::msg::Image>& last_msg) override;

private:
  std::chrono::steady_clock::time_point last_msg_time_;
};

#endif // IMAGE_ALIVE_CONDITION_HPP
