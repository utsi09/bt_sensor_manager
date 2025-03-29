//sensor_manager_main.cpp
#include <rclcpp/rclcpp.hpp>
#include <ament_index_cpp/get_package_share_directory.hpp>
#include "behaviortree_cpp/loggers/groot2_publisher.h"

// BehaviorTree
#include <behaviortree_cpp/bt_factory.h>
#include <behaviortree_ros2/bt_topic_sub_node.hpp>

// 커스텀 BT 노드
#include "bt_sensor_manager/restart_sensor_action.hpp"
#include "bt_sensor_manager/topic_alive_condition.hpp"
#include "bt_sensor_manager/image_alive_condition.hpp"

using namespace std::chrono_literals;
//branch test 테스트 브랜치 테스트2-commit
int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<rclcpp::Node>("sensor_manager_node");

  BT::BehaviorTreeFactory factory;
  BT::RosNodeParams ros_params(node);

  factory.registerNodeType<ImageAliveCondition>("ImageAliveCondition", ros_params);
  factory.registerNodeType<TopicAliveCondition>("TopicAliveCondition", ros_params);
  factory.registerNodeType<RestartSensorAction>("RestartSensorAction");

//   std::string tree_path = ament_index_cpp::get_package_share_directory("my_pkg") 
//                         + "/behavior_trees/monitor_tree.xml";
std::string tree_path =
  ament_index_cpp::get_package_share_directory("bt_sensor_manager") +
  "/behavior_trees/monitor_tree.xml";


  
  auto blackboard = BT::Blackboard::create();
  BT::Tree tree = factory.createTreeFromFile(tree_path, blackboard);

  BT::Groot2Publisher groot_publisher(tree, 1666);//groot 퍼블리셔

  rclcpp::Rate loop_rate(10);
  while(rclcpp::ok()) {

    rclcpp::spin_some(node);
    tree.tickWhileRunning(10ms);
    loop_rate.sleep();
  }
  

  rclcpp::shutdown();
  return 0;
}
