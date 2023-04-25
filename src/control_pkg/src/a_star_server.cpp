#include "rclcpp/rclcpp.hpp"
#include "interfaces/srv/platform_vel.hpp"
#include <interfaces/msg/motor_vels.hpp>

#include <memory>
#include <cinttypes>

rclcpp::Node::SharedPtr AStarServer = nullptr;

void a_star_caller(const std::shared_ptr<interfaces::srv::PlatformVel::Request> request,
          std::shared_ptr<interfaces::srv::PlatformVel::Response>      response)
		{
		  
			
		  	// sent=sent+1;
			response-> sent;
		}


int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

 
  AStarServer = rclcpp::Node::make_shared("a_star_server");


  rclcpp::Service<interfaces::srv::PlatformVel>::SharedPtr service =
    AStarServer->create_service<interfaces::srv::PlatformVel>("a_star_service", &a_star_caller);


  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Ready to send velocities to platform 1.");

  rclcpp::spin(AStarServer);
  rclcpp::shutdown();
}