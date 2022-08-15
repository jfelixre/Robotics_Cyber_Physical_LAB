#include "rclcpp/rclcpp.hpp"
#include "interfaces/srv/platform_vel.hpp"
#include <interfaces/msg/img_data.hpp>
#include <interfaces/srv/control_pid_start.hpp>

#include <memory>
#include <cinttypes>
using std::placeholders::_1;

rclcpp::Node::SharedPtr Control_PID_P1_server = nullptr;
//rclcpp::Publisher<interfaces::msg::MotorVels>::SharedPtr publisher_M = nullptr;
rclcpp::Subscription<interfaces::msg::ImgData>::SharedPtr subs_img_data= nullptr;



void subs_callback(const interfaces::msg::ImgData::SharedPtr data_msg)
{
	RCLCPP_INFO(Control_PID_P1_server->get_logger(), "Data received" );
}



void pid_serv(const std::shared_ptr<interfaces::srv::ControlPidStart::Request> request,
		std::shared_ptr<interfaces::srv::ControlPidStart::Response> response)
{
	

	RCLCPP_INFO(Control_PID_P1_server->get_logger(), "Listo" );


	subs_img_data = Control_PID_P1_server->create_subscription<interfaces::msg::ImgData>(
		"image_data", 1, std::bind(&subs_callback,Control_PID_P1_server, _1));
}






int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);


  Control_PID_P1_server = rclcpp::Node::make_shared("control_pid_p1_server");

  rclcpp::Service<interfaces::srv::ControlPidStart>::SharedPtr service = 
  	Control_PID_P1_server->create_service<interfaces::srv::ControlPidStart>("control_pid_p1", &pid_serv);

  	

  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Ready to start PID control on platform 1.");

  rclcpp::spin(Control_PID_P1_server);

  rclcpp::shutdown();
}