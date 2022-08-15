#include "rclcpp/rclcpp.hpp"
#include "interfaces/srv/platform_vel.hpp"
#include <geometry_msgs/msg/vector3.hpp>

#include <memory>
#include <cinttypes>

rclcpp::Node::SharedPtr Platform1VelServer_node = nullptr;

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

		  	geometry_msgs::msg::Vector3 vel_M1;
		  	geometry_msgs::msg::Vector3 vel_M2;
		  	geometry_msgs::msg::Vector3 vel_M3;
		  	geometry_msgs::msg::Vector3 vel_M4;


		 	vel_M1.x=(velx + vely + ((La+Lb)*velang))/R;
		  	vel_M2.x=(-velx + vely - ((La+Lb)*velang))/R;
		  	vel_M3.x=(-velx + vely + ((La+Lb)*velang))/R;
		  	vel_M4.x=(+velx + vely - ((La+Lb)*velang))/R;

		  	rclcpp::Publisher<geometry_msgs::msg::Vector3>::SharedPtr publisher_M1 = Platform1VelServer_node->create_publisher<
  				geometry_msgs::msg::Vector3>("/robot_platform_1/vel_M1",1);

  			rclcpp::Publisher<geometry_msgs::msg::Vector3>::SharedPtr publisher_M2 = Platform1VelServer_node->create_publisher<
  				geometry_msgs::msg::Vector3>("/robot_platform_1/vel_M2",1);

  			rclcpp::Publisher<geometry_msgs::msg::Vector3>::SharedPtr publisher_M3 = Platform1VelServer_node->create_publisher<
  				geometry_msgs::msg::Vector3>("/robot_platform_1/vel_M3",1);

  			rclcpp::Publisher<geometry_msgs::msg::Vector3>::SharedPtr publisher_M4 = Platform1VelServer_node->create_publisher<
  				geometry_msgs::msg::Vector3>("/robot_platform_1/vel_M4",1);



		  	publisher_M1->publish(vel_M1);
		  	publisher_M2->publish(vel_M2);
		  	publisher_M3->publish(vel_M3);
		  	publisher_M4->publish(vel_M4);

		  	RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Velocities sent to platform 1:  M1=%f M2=%f M3=%f M4=%f",
		  		vel_M1.x, vel_M2.x, vel_M3.x, vel_M4.x);

		  	sent=sent+1;
			response-> sent;
		}


int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

 
  Platform1VelServer_node = rclcpp::Node::make_shared("platform1_vel_server");


  rclcpp::Service<interfaces::srv::PlatformVel>::SharedPtr service =
    Platform1VelServer_node->create_service<interfaces::srv::PlatformVel>("platform1_vel", &send_vel);


  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Ready to send velocities to platform 1.");

  rclcpp::spin(Platform1VelServer_node);
  rclcpp::shutdown();
}