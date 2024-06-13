#include "rclcpp/rclcpp.hpp"
#include <interfaces/msg/robot_state.hpp>
#include <interfaces/msg/positions.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <interfaces/msg/robot_objective.hpp>
#include <geometry_msgs/msg/polygon.hpp>
#include <interfaces/srv/trajectory_control.hpp>
#include <interfaces/srv/platform_vel.hpp>
#include <interfaces/msg/limit_switch.hpp>
#include <interfaces/msg/motor_vels_w_arm.hpp>
#include <interfaces/msg/arm_joints_positions.hpp>
#include <interfaces/msg/motor_arm_vels.hpp>
#include <std_msgs/msg/float64.h>
#include <std_msgs/msg/float64.hpp>

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

int robot_id = 0;
std_msgs::msg::Float64 msg_b1;
std_msgs::msg::Float64 msg_b2;
std_msgs::msg::Float64 msg_b3;
std_msgs::msg::Float64 msg_p1;
std_msgs::msg::Float64 msg_p2;

bool home_pos = true;

int k = 0; 


class Arm_Position_Node : public rclcpp::Node
{
	public:
		Arm_Position_Node() : Node("arm_position_node")
		{
            this->declare_parameter<int>("robot_id", 0);
            robot_id = this->get_parameter("robot_id").as_int();
            RCLCPP_INFO(this->get_logger(), "Received Robot_ID: %d", robot_id);

			std::stringstream ss_topic_name;
            ss_topic_name << "/robot_0" << robot_id << "/set_arm_position";
            std::string topic_name = ss_topic_name.str();

            arm_position_subs= create_subscription<interfaces::msg::ArmJointsPositions>(
      			topic_name, 1, std::bind(&Arm_Position_Node::arm_pos_callback,this,_1));
            


            timer_pid_cb_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);

            timer_pid_ = this->create_wall_timer(
                100ms, std::bind(&Arm_Position_Node::timer_pid_callback, this), timer_pid_cb_group_);

            

            std::stringstream ss_topic_name_b1;
            ss_topic_name_b1 << "/robot_0" << robot_id << "/b1_joint/controller";
            std::string topic_name_b1 = ss_topic_name_b1.str();
                     
            publisher_pos_b1 = this->create_publisher<std_msgs::msg::Float64>(topic_name_b1,10);

            std::stringstream ss_topic_name_b2;
            ss_topic_name_b2 << "/robot_0" << robot_id << "/b2_joint/controller";
            std::string topic_name_b2 = ss_topic_name_b2.str();

            publisher_pos_b2 = this->create_publisher<std_msgs::msg::Float64>(topic_name_b2,10);

            std::stringstream ss_topic_name_b3;
            ss_topic_name_b3 << "/robot_0" << robot_id << "/b3_joint/controller";
            std::string topic_name_b3 = ss_topic_name_b3.str();

            publisher_pos_b3 = this->create_publisher<std_msgs::msg::Float64>(topic_name_b3,10);

            std::stringstream ss_topic_name_p1;
            ss_topic_name_p1 << "/robot_0" << robot_id << "/p1_joint/controller";
            std::string topic_name_p1 = ss_topic_name_p1.str();

            publisher_pos_p1 = this->create_publisher<std_msgs::msg::Float64>(topic_name_p1,10);

            std::stringstream ss_topic_name_p2;
            ss_topic_name_p2 << "/robot_0" << robot_id << "/p2_joint/controller";
            std::string topic_name_p2 = ss_topic_name_p2.str();

            publisher_pos_p2 = this->create_publisher<std_msgs::msg::Float64>(topic_name_p2,10);
        }

    private:

        rclcpp::TimerBase::SharedPtr timer_pid_;
        rclcpp::CallbackGroup::SharedPtr timer_pid_cb_group_;
        rclcpp::Subscription<interfaces::msg::ArmJointsPositions>::SharedPtr arm_position_subs;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_pos_b1;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_pos_b2;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_pos_b3;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_pos_p1;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_pos_p2;



        void timer_pid_callback()   //CONTROL PID//
        { 
            if (home_pos==true){
                msg_b1.data = 2.0;
                msg_b2.data = -2.0;
                msg_b3.data = 2.0;
                msg_p1.data = -0.5;
                msg_p2.data = msg_p1.data;

                publisher_pos_b1->publish(msg_b1);
                publisher_pos_b2->publish(msg_b2);
                publisher_pos_b3->publish(msg_b3);
                publisher_pos_p1->publish(msg_p1);
                publisher_pos_p2->publish(msg_p2);
                k++;
                if (k>100){
                    home_pos = false;
                }

            }


        }

        void arm_pos_callback(const interfaces::msg::ArmJointsPositions::SharedPtr msg)   //CONTROL PID//
        {

        }

};



int main(int argc, char * argv[])
{


    rclcpp::init(argc, argv);

	auto arm_position_node = std::make_shared<Arm_Position_Node>();
    

    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(arm_position_node);
    executor.spin();

 	rclcpp::shutdown();
  
}