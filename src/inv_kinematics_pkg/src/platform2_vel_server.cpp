#include "rclcpp/rclcpp.hpp"
#include "interfaces/srv/platform_vel.hpp"
#include <interfaces/msg/motor_vels.hpp>

#include <memory>
#include <cinttypes>

rclcpp::Node::SharedPtr Platform2VelServer_node = nullptr;
rclcpp::Publisher<interfaces::msg::MotorVels>::SharedPtr publisher_M = nullptr;

void send_vel(const std::shared_ptr<interfaces::srv::PlatformVel::Request> request,
          std::shared_ptr<interfaces::srv::PlatformVel::Response>      response)
		{
		  
			float velx = request->x_vel;
			float vely = request->y_vel;
			float velang = request-> ang_vel;
			
			int sent=0;

			float La = 128.9375;
		  	float Lb = 109.379;
		  	float R = 100/2;

		  	interfaces::msg::MotorVels vel_M;


		 		vel_M.vel_m1=(velx + vely + ((La+Lb)*velang))/R;
		  	vel_M.vel_m2=(-velx + vely - ((La+Lb)*velang))/R;
		  	vel_M.vel_m3=(-velx + vely + ((La+Lb)*velang))/R;
		  	vel_M.vel_m4=(+velx + vely - ((La+Lb)*velang))/R;


  			publisher_M->publish(vel_M);


		  	RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Velocities sent to platform 2:  M1=%f M2=%f M3=%f M4=%f",
		  		vel_M.vel_m1, vel_M.vel_m2, vel_M.vel_m3, vel_M.vel_m4);

		  	sent=sent+1;
			response-> sent;
		}


int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

 
  Platform2VelServer_node = rclcpp::Node::make_shared("platform2_vel_server");


  rclcpp::Service<interfaces::srv::PlatformVel>::SharedPtr service =
    Platform2VelServer_node->create_service<interfaces::srv::PlatformVel>("platform2_vel", &send_vel);

  publisher_M = Platform2VelServer_node->create_publisher<interfaces::msg::MotorVels>("/robot_platform_2/vel_Motors",1);

  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Ready to send velocities to platform 2.");

  rclcpp::spin(Platform2VelServer_node);
  rclcpp::shutdown();
}