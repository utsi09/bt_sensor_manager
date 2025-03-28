#ifndef RESTART_SENSOR_ACTION_HPP
#define RESTART_SENSOR_ACTION_HPP

#include <behaviortree_cpp/bt_factory.h>
#include <rclcpp/rclcpp.hpp>
#include <cstdlib>

class RestartSensorAction : public BT::SyncActionNode
{
public:
  RestartSensorAction(const std::string & name, const BT::NodeConfiguration & config)
    : BT::SyncActionNode(name, config) {}

  static BT::PortsList providedPorts()
  {
    return {
      BT::InputPort<std::string>("cmd"),
      BT::InputPort<std::string>("topic_name"),
      BT::InputPort<std::string>("sensor_type") // "image" 또는 "pointcloud2" 등
    };
  }

  BT::NodeStatus tick() override;
};

#endif
