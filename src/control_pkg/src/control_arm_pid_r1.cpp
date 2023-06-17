#include "rclcpp/rclcpp.hpp"
#include <interfaces/msg/robot_state.hpp>
#include <interfaces/msg/positions.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_sensor_msgs/tf2_sensor_msgs.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <interfaces/msg/robot_objective.hpp>
#include <geometry_msgs/msg/polygon.hpp>
#include <interfaces/srv/trajectory_control.hpp>
#include <interfaces/srv/platform_vel.hpp>
#include <Eigen/Dense>

#include <memory>
#include <cinttypes>
#include <chrono>
#include <functional>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

using std::placeholders::_1;
using namespace std::chrono_literals;



class Control_Arm_Pid_R1 : public rclcpp::Node
{
	public:
		Control_Arm_Pid_R1() : Node("node_control_arm_pid_r1")
		{
                       

        }

    private:

};

class Node_Estimate_Position : public rclcpp::Node
{
	public:
		Node_Estimate_Position() : Node("node_estimate_position")
		{
                       

        }

    private:

};


int main(int argc, char * argv[])
{


    rclcpp::init(argc, argv);

	auto node_control_arm_pid_r1 = std::make_shared<Control_Arm_Pid_R1>();
    auto node_estimate_position = std::make_shared<Node_Estimate_Position>();
	//auto node_subs_positions = std::make_shared<Node_Subs_Positions>();
   // auto node_control_timer = std::make_shared<Node_Control_Timer>();
   // auto node_client_vel = std::make_shared<Node_Client_Vel>();

    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node_control_arm_pid_r1);
   // executor.add_node(node_subs_path);
	//executor.add_node(node_subs_positions);
   // executor.add_node(node_control_timer);
    //executor.add_node(node_client_vel);
    executor.spin();

 	rclcpp::shutdown();
  
}