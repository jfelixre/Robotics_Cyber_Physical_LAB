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
#include <interfaces/msg/limit_switch.hpp>
#include <interfaces/msg/motor_vels_w_arm.hpp>
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

//Variables LimitSwitch
int LS_B1_min, LS_B1_max, LS_B2_min, LS_B2_max, LS_B3_min, LS_B3_max;

//Variables velocidades de encoders
float vel_B1,vel_B2,vel_B3,vel_G1;

//Variables de posición de brazo
float B1_pos=0;
float B2_pos=0;
float B3_pos=0;
float velG1=1;


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
            LmSw_subscriber= create_subscription<interfaces::msg::LimitSwitch>(
      			"/robot_1/limit_switch", 1, std::bind(&Node_Estimate_Position::LmSw_subs,this,_1));

			encoders_subscriber= create_subscription<interfaces::msg::MotorVelsWArm>(
      			"/robot_1/encoders", 1, std::bind(&Node_Estimate_Position::encoders_subs,this,_1));

            timer_ep_cb_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);

            timer_estimate_pos_ = this->create_wall_timer(
                 100ms, std::bind(&Node_Estimate_Position::timer_ep_callback, this), timer_ep_cb_group_);
                       
        }

    private:

    rclcpp::Subscription<interfaces::msg::LimitSwitch>::SharedPtr LmSw_subscriber;
    rclcpp::Subscription<interfaces::msg::MotorVelsWArm>::SharedPtr encoders_subscriber;
    rclcpp::TimerBase::SharedPtr timer_estimate_pos_;
    rclcpp::CallbackGroup::SharedPtr timer_ep_cb_group_;

     void timer_ep_callback()
    { 
        //Cálculo de posiciones
		//B1_pos=B1_pos+(vel_B1*dt_sec);
		B1_pos=vel_B1+0.1;
		//B2_pos=B2_pos+(vel_B2*dt_sec);
		B2_pos=(vel_B2+0.1)*-1;
		//B3_pos=B3_pos+(vel_B3*dt_sec);
		B3_pos=vel_B3+0.1;

    }

    void LmSw_subs(const interfaces::msg::LimitSwitch::SharedPtr msg) const
		{
			LS_B1_min=msg->b1_min;
			LS_B1_max=msg->b1_max;
			LS_B2_min=msg->b2_min;
			LS_B2_max=msg->b2_max;
			LS_B3_min=msg->b3_min;
			LS_B3_max=msg->b3_max;

			if (LS_B1_min==1) { 
				B1_pos=-1.4;
			}

			if (LS_B1_max==1) { 
				B1_pos=1.4;
			}

			if (LS_B2_min==1) { 
				B2_pos=-1.45;
			}

			if (LS_B2_max==1) { 
				B2_pos=1.45;
			}

			if (LS_B3_min==1) { 
				B3_pos=-1.5;
			}

			if (LS_B3_max==1) { 
				B3_pos=1.5;
			}

		}

		void encoders_subs(const interfaces::msg::MotorVelsWArm::SharedPtr msg) const
		{
			vel_B1=msg->vel_b1;
			vel_B2=msg->vel_b2;
			vel_B3=msg->vel_b3;
			vel_G1=msg->vel_g1;

			vel_B2=vel_B2*-1;
		}


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