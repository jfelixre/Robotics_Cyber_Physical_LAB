#include "rclcpp/rclcpp.hpp"
#include "interfaces/srv/robot_vel.hpp"
#include <interfaces/msg/motor_vels_w_arm.hpp>

#include <memory>
#include <cinttypes>

rclcpp::Node::SharedPtr Robot1VelServer_node = nullptr;
rclcpp::Publisher<interfaces::msg::MotorVelsWArm>::SharedPtr publisher_M = nullptr;

void send_vel(const std::shared_ptr<interfaces::srv::RobotVel::Request> request,
          std::shared_ptr<interfaces::srv::RobotVel::Response>      response)
		{
		  
			float velx = request->x_vel;
			float vely = request->y_vel;
			float velang = request-> ang_vel;
				
			int sent=0;

			float La = 128.9375;
			float Lb = 109.379;
			float R = 100/2;

			interfaces::msg::MotorVelsWArm vel_M;

			/*
			vel_M.vel_m1=(velx + vely + ((La+Lb)*velang))/R;
			vel_M.vel_m2=(-velx + vely - ((La+Lb)*velang))/R;
			vel_M.vel_m3=(-velx + vely + ((La+Lb)*velang))/R;
			vel_M.vel_m4=(velx + vely - ((La+Lb)*velang))/R;
				*/


			vel_M.vel_m1=(velx - vely - ((La+Lb)*velang))/R * -1;
			vel_M.vel_m2=(velx + vely + ((La+Lb)*velang))/R * -1;
			vel_M.vel_m3=(velx + vely - ((La+Lb)*velang))/R * -1;
			vel_M.vel_m4=(velx - vely + ((La+Lb)*velang))/R * -1;

			if (vel_M.vel_m1 > 20) { vel_M.vel_m1 = 20;}
			if (vel_M.vel_m1 < -20) { vel_M.vel_m1 = -20;}
			if (vel_M.vel_m2 > 20) { vel_M.vel_m2 = 20;}
			if (vel_M.vel_m2 < -20) { vel_M.vel_m2 = -20;}
			if (vel_M.vel_m3 > 20) { vel_M.vel_m3 = 20;}
			if (vel_M.vel_m3 < -20) { vel_M.vel_m3 = -20;}
			if (vel_M.vel_m4 > 20) { vel_M.vel_m4 = 20;}
			if (vel_M.vel_m4 < -20) { vel_M.vel_m4 = -20;}



			vel_M.vel_b1 = request->b1_vel;
			vel_M.vel_b2 = request->b2_vel;
			vel_M.vel_b3 = request->b3_vel;
			vel_M.vel_g1 = request->g1_vel;


			publisher_M->publish(vel_M);


			RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Velocities sent to robot 1:  M1=%f M2=%f M3=%f M4=%f B1=%f B2=%f B3=%f G1=%f",
				vel_M.vel_m1, vel_M.vel_m2, vel_M.vel_m3, vel_M.vel_m4, vel_M.vel_b1, vel_M.vel_b2, vel_M.vel_b3, vel_M.vel_g1);

			sent=sent+1;
			response-> sent;
		}


int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

 
  Robot1VelServer_node = rclcpp::Node::make_shared("robot1_vel_server");


  rclcpp::Service<interfaces::srv::RobotVel>::SharedPtr service =
    Robot1VelServer_node->create_service<interfaces::srv::RobotVel>("robot1_vel", &send_vel);

  publisher_M = Robot1VelServer_node->create_publisher<interfaces::msg::MotorVelsWArm>("/robot_1/vel_Motors",1);

  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Ready to send velocities to Robot 1.");

  rclcpp::spin(Robot1VelServer_node);
  rclcpp::shutdown();
}