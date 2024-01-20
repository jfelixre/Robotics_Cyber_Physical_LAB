#include "rclcpp/rclcpp.hpp"
#include <rclcpp/parameter.hpp>
#include <interfaces/msg/robot_state.hpp>
#include <interfaces/msg/positions.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_sensor_msgs/tf2_sensor_msgs.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <interfaces/msg/robot_objective.hpp>
#include <interfaces/srv/event_control.hpp>
#include <interfaces/msg/arm_joints_positions.hpp>
#include <interfaces/msg/trajectory_control.hpp>
#include <interfaces/msg/task_description.hpp>
#include <interfaces/msg/robot_state.hpp>

#include <memory>
#include <cinttypes>
#include <chrono>
#include <functional>
#include <cmath>
#include <iostream>
#include <fstream>

using std::placeholders::_1;
using namespace std::chrono_literals;
using namespace std;

int robot_id = 0;
interfaces::msg::TaskDescription task;
interfaces::msg::Positions tag_positions;

class Event_Driven_Control : public rclcpp::Node
{
	public:
		Event_Driven_Control() : Node("event_driven_control")
		{
            this->declare_parameter<int>("robot_id", 0);
            robot_id = this->get_parameter("robot_id").as_int();
            RCLCPP_INFO(this->get_logger(), "Received Robot_ID: %d", robot_id);

            std::stringstream ss_topic_name;
            ss_topic_name << "/robot_" << robot_id << "/task_assigned";
            std::string topic_name = ss_topic_name.str();

            subscription_task_robot = this->create_subscription<interfaces::msg::TaskDescription>(
                topic_name, 10, std::bind(&Event_Driven_Control::task_robot_callback, this, _1));

            subscription_positions = this->create_subscription<interfaces::msg::Positions>(
                "positions", 10, std::bind(&Event_Driven_Control::positions_callback, this, _1));

		}


	private:

        void task_robot_callback(interfaces::msg::TaskDescription::SharedPtr msg)
            {
                task = *msg;
                RCLCPP_INFO(this->get_logger(), "Task ID %d received by event_control of Robot %d", task.task_id, robot_id);

                //INICIAR TAREA
            }

        void positions_callback(interfaces::msg::Positions::SharedPtr msg)
            {
                tag_positions = *msg;
                
            }

        rclcpp::Subscription<interfaces::msg::TaskDescription>::SharedPtr subscription_task_robot;
        rclcpp::Subscription<interfaces::msg::Positions>::SharedPtr subscription_positions;
};




int main(int argc, char * argv[])
{
  	rclcpp::init(argc, argv);

	 auto node = std::make_shared<Event_Driven_Control>();
    // auto node_subs_pose_r1 = std::make_shared<Node_Subs_Pose_R1>();
	// auto node_subs_state_r2 = std::make_shared<Node_Subs_State_R2>();

     rclcpp::executors::MultiThreadedExecutor executor;
     executor.add_node(node);
    // executor.add_node(node_subs_pose_r1);
	// executor.add_node(node_subs_state_r2);
     executor.spin();

 	rclcpp::shutdown();
  
}