#ifndef TOPIC_ALIVE_CONDITION_HPP
#define TOPIC_ALIVE_CONDITION_HPP

#include <behaviortree_ros2/bt_topic_sub_node.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <rclcpp/rclcpp.hpp>
#include <chrono>

using namespace std::chrono_literals;

class TopicAliveCondition : public BT::RosTopicSubNode<sensor_msgs::msg::PointCloud2>
{
public:
  TopicAliveCondition(const std::string & name, const BT::NodeConfig & config, const BT::RosNodeParams & params)
    : BT::RosTopicSubNode<sensor_msgs::msg::PointCloud2>(name, config, params)
  {
    last_msg_time_ = std::chrono::steady_clock::now();
  }

  static BT::PortsList providedPorts()
  {
    return { BT::InputPort<std::string>("topic_name") };
  }

protected:
  BT::NodeStatus onTick(const std::shared_ptr<sensor_msgs::msg::PointCloud2> & last_msg) override;

private:
  std::chrono::steady_clock::time_point last_msg_time_;
};

#endif // TOPIC_ALIVE_CONDITION_HPP
