#include "rclcpp/rclcpp.hpp"
#include <rclcpp/parameter.hpp>
#include <interfaces/msg/robot_state.hpp>
#include <interfaces/msg/positions.hpp>
#include <tf2_ros/transform_listener.h>
#include "tf2_ros/buffer.h"
#include <interfaces/msg/robot_objective.hpp>
#include <interfaces/srv/event_control.hpp>
#include <interfaces/msg/arm_joints_positions.hpp>
#include <interfaces/msg/trajectory_control.hpp>
#include <interfaces/msg/task_description.hpp>
#include <interfaces/msg/robot_state.hpp>
#include <interfaces/msg/control_finish.hpp>
#include <interfaces/msg/robot_objective.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <sensor_msgs/msg/joint_state.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>

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




class Transform_To_Tf : public rclcpp::Node
{
	public:
		Transform_To_Tf() : Node("transform_to_tf")
		{
            this->declare_parameter<int>("robot_id", 1);
            robot_id = this->get_parameter("robot_id").as_int();
            //RCLCPP_INFO(this->get_logger(), "Received Robot_ID: %d", robot_id);

            std::stringstream ss_topic_name;
            ss_topic_name << "/joint_states_robot_0" << robot_id;
            std::string topic_name = ss_topic_name.str();

        
     		
      		joint_subscriber= create_subscription<sensor_msgs::msg::JointState>(
      			topic_name, 1, std::bind(&Transform_To_Tf::joint_subs,this,_1));
			
            tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(*this);
		}

	private:

		void joint_subs(const sensor_msgs::msg::JointState::SharedPtr msg) const
		{
            geometry_msgs::msg::TransformStamped transform_stamped;
            transform_stamped.header.stamp = now();
            transform_stamped.header.frame_id = "marker_id_01";

            //Iterate over the joints and create a transform for each joint
            for (int i = 0; i < msg->name.size(); i++)
            {
                transform_stamped.child_frame_id = "robot_01/base_link";
                double z_rot = msg->position[i];

                //Create a quaternion from the z rotation
                tf2::Quaternion q;
                q.setRPY(0, 0, z_rot);
                
                transform_stamped.transform.rotation.x = q.x();
                transform_stamped.transform.rotation.y = q.y();
                transform_stamped.transform.rotation.z = q.z();
                transform_stamped.transform.rotation.w = q.w();

                tf_broadcaster_->sendTransform(transform_stamped);

            }
                
		}



		rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr joint_subscriber;
        std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;


};




int main(int argc, char * argv[])
{
  	rclcpp::init(argc, argv);
  	
	auto transform_to_tf = std::make_shared<Transform_To_Tf>();
    

    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(transform_to_tf);
    executor.spin();

 	rclcpp::shutdown();
  
}